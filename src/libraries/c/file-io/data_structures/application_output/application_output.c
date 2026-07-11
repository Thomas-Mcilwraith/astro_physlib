/*
 * application_output.c
 *
 * Author: Thomas McIlwraith
 * Date: 18/04/2026
 */

#include "application_output.h"

StatusCode application_output_read_json(
    // Outputs
    application_output_t *application_output,
    // Inputs
    const char* working_directory,
    const char* run_title,
    const char* program_name) {

    // Local variables
    StatusCode status = OK;
    cJSON *json = NULL;
    char filename[FILE_NAME_BUFFER_SIZE], filepath[FULL_PATH_BUFFER_SIZE];
    int len_filename;

    if (application_output == NULL) {
        LOG(ERROR, "Bad pointer to application_output_read");
        return ERROR;
    }

    len_filename = snprintf(filename, FILE_NAME_BUFFER_SIZE, "%s_%s.json", run_title, program_name);
    if (len_filename >= FILE_NAME_BUFFER_SIZE) {
        LOG(ERROR, "Filename too long: %s", filename);
        return ERROR;
    }
    status = working_area_path(filepath, working_directory, OUTPUTS, filename, FULL_PATH_BUFFER_SIZE);
    if (status != OK) {
        LOG(ERROR, "Error constructing path to application output file: %s", filename);
        return status;
    }

    status = read_json(&json, filepath);
    if (status != OK) {
        LOG(ERROR, "Failed to read JSON file: %s", filepath);
        return ERROR;
    }

    // Assign the variable length arrays
    cJSON *TLE = cJSON_GetObjectItemCaseSensitive(json, "TLE");
    if (cJSON_IsArray(TLE) == false) {
        LOG(ERROR, "TLE is missing from output file");
        cJSON_Delete(json);
        return ERROR;
    }
    cJSON *EPHM = cJSON_GetObjectItemCaseSensitive(json, "EPHM");
    if (cJSON_IsArray(EPHM) == false) {
        LOG(ERROR, "EPHM is missing from output file");
        cJSON_Delete(json);
        return ERROR;
    }
    cJSON *TSPN = cJSON_GetObjectItemCaseSensitive(json, "TSPN");
    if (cJSON_IsArray(TSPN) == false) {
        LOG(ERROR, "TSPN is missing from output file");
        cJSON_Delete(json);
        return ERROR;
    }

    // Assign all the static values
    application_output->n_TLE = cJSON_GetArraySize(TLE);
    application_output->n_EPHM = cJSON_GetArraySize(EPHM);
    application_output->n_TSPN = cJSON_GetArraySize(TSPN);

    if (application_output->n_TLE > MAX_OUTPUT_FILES) {
        LOG(ERROR, "Detected %d TLE files from %s %s, The maximum is %d",
                application_output->n_TLE, program_name, run_title, MAX_OUTPUT_FILES);
        cJSON_Delete(json);
        application_output_free(application_output);
        return ERROR;
    } else if (application_output->n_EPHM > MAX_OUTPUT_FILES) {
        LOG(ERROR, "Detected %d EPHM files from %s %s, The maximum is %d",
                application_output->n_EPHM, program_name, run_title, MAX_OUTPUT_FILES);
        cJSON_Delete(json);
        application_output_free(application_output);
        return ERROR;
    } else if (application_output->n_TSPN > MAX_OUTPUT_FILES) {
        LOG(ERROR, "Detected %d TSPN files from %s %s, The maximum is %d",
                application_output->n_TSPN, program_name, run_title, MAX_OUTPUT_FILES);
        cJSON_Delete(json);
        application_output_free(application_output);
        return ERROR;
    }

    // Load all the arrays
    for (int i = 0; i < application_output->n_TLE; i++) {
        application_output->TLE[i] = strdup(cJSON_GetArrayItem(TLE, i)->valuestring);
        if (application_output->TLE[i] == NULL) {
            LOG(ERROR, "Failed to allocate memory for TLE");
            cJSON_Delete(json);
            application_output_free(application_output);
            return ERROR;
        }
    }
    for (int i = 0; i < application_output->n_EPHM; i++) {
        application_output->EPHM[i] = strdup(cJSON_GetArrayItem(EPHM, i)->valuestring);
        if (application_output->EPHM[i] == NULL) {
            LOG(ERROR, "Failed to allocate memory for EPHM");
            cJSON_Delete(json);
            application_output_free(application_output);
            return ERROR;
        }
    }
    for (int i = 0; i < application_output->n_TSPN; i++) {
        application_output->TSPN[i] = strdup(cJSON_GetArrayItem(TSPN, i)->valuestring);
        if (application_output->TSPN[i] == NULL) {
            LOG(ERROR, "Failed to allocate memory for TSPN");
            cJSON_Delete(json);
            application_output_free(application_output);
            return ERROR;
        }
    }

    // Free memory
    cJSON_Delete(json);

    return OK;
}

StatusCode application_output_free(application_output_t *application_output) {
    if (application_output == NULL) {
        return ERROR;
    }

    for (int i = 0; i < application_output->n_TLE; i++) {
        free(application_output->TLE[i]);
        application_output->TLE[i] = NULL;
    }

    for (int i = 0; i < application_output->n_EPHM; i++) {
        free(application_output->EPHM[i]);
        application_output->EPHM[i] = NULL;
    }

    for (int i = 0; i < application_output->n_TSPN; i++) {
        free(application_output->TSPN[i]);
        application_output->TSPN[i] = NULL;
    }

    application_output->n_TLE = 0;
    application_output->n_EPHM = 0;
    application_output->n_TSPN = 0;

    return OK;
}

