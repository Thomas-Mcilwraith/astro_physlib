/*
 * application_output.c
 *
 * Author: Thomas McIlwraith
 * Date: 18/04/2026
 */

#include "application_output.h"

StatusCode application_output_read(
    // Outputs
    application_output_t *application_output,
    // Inputs
    const char* working_directory,
    const char* run_title) {

    // Local variables
    StatusCode status = OK;
    cJSON *json = NULL;
    char filename[FILE_NAME_BUFFER_SIZE], filepath[FULL_PATH_BUFFER_SIZE];
    int len_filename;

    if (application_output == NULL) {
        LOG(ERROR, "Bad pointer to application_output_read");
        return ERROR;
    }

    len_filename = snprintf(filename, FILE_NAME_BUFFER_SIZE, "%s_outputs.json", run_title);
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
    cJSON *aTLE = cJSON_GetObjectItemCaseSensitive(json, "aTLE");
    if (cJSON_IsArray(aTLE) == false) {
        LOG(ERROR, "TLE is missing from output file");
        cJSON_Delete(json);
        return ERROR;
    }
    cJSON *aEPHM = cJSON_GetObjectItemCaseSensitive(json, "aEPHM");
    if (cJSON_IsArray(aEPHM) == false) {
        LOG(ERROR, "EPHM is missing from output file");
        cJSON_Delete(json);
        return ERROR;
    }
    cJSON *aTSPN = cJSON_GetObjectItemCaseSensitive(json, "aTSPN");
    if (cJSON_IsArray(aTSPN) == false) {
        LOG(ERROR, "TSPN is missing from output file");
        cJSON_Delete(json);
        return ERROR;
    }

    // Assign all the static values
    application_output->n_TLE = cJSON_GetArraySize(aTLE);
    application_output->n_EPHM = cJSON_GetArraySize(aEPHM);
    application_output->n_TSPN = cJSON_GetArraySize(aTSPN);

    if (application_output->n_TLE > MAX_OUTPUT_FILES) {
        LOG(ERROR, "Detected %d TLE files from program %s, The maximum is %d",
                application_output->n_TLE, run_title, MAX_OUTPUT_FILES);
        cJSON_Delete(json);
        application_output_free(application_output);
        return ERROR;
    } else if (application_output->n_EPHM > MAX_OUTPUT_FILES) {
        LOG(ERROR, "Detected %d EPHM files from program %s, The maximum is %d",
                application_output->n_EPHM, run_title, MAX_OUTPUT_FILES);
        cJSON_Delete(json);
        application_output_free(application_output);
        return ERROR;
    } else if (application_output->n_TSPN > MAX_OUTPUT_FILES) {
        LOG(ERROR, "Detected %d TSPN files from program %s, The maximum is %d",
                application_output->n_TSPN, run_title, MAX_OUTPUT_FILES);
        cJSON_Delete(json);
        application_output_free(application_output);
        return ERROR;
    }

    // Load all the arrays
    for (int i = 0; i < application_output->n_TLE; i++) {
        application_output->aTLE[i] = strdup(cJSON_GetArrayItem(aTLE, i)->valuestring);
        if (application_output->aTLE[i] == NULL) {
            LOG(ERROR, "Failed to allocate memory for TLE");
            cJSON_Delete(json);
            application_output_free(application_output);
            return ERROR;
        }
    }
    for (int i = 0; i < application_output->n_EPHM; i++) {
        application_output->aEPHM[i] = strdup(cJSON_GetArrayItem(aEPHM, i)->valuestring);
        if (application_output->aEPHM[i] == NULL) {
            LOG(ERROR, "Failed to allocate memory for EPHM");
            cJSON_Delete(json);
            application_output_free(application_output);
            return ERROR;
        }
    }
    for (int i = 0; i < application_output->n_TSPN; i++) {
        application_output->aTSPN[i] = strdup(cJSON_GetArrayItem(aTSPN, i)->valuestring);
        if (application_output->aTSPN[i] == NULL) {
            LOG(ERROR, "Failed to allocate memory for TSPN");
            cJSON_Delete(json);
            application_output_free(application_output);
            return ERROR;
        }
    }

    // Write the run title into the application_output struct
    application_output->run_title = strdup(run_title);

    // Free memory
    cJSON_Delete(json);

    return OK;
}

StatusCode application_output_free(application_output_t *application_output) {
    if (application_output == NULL) {
        return ERROR;
    }

    for (int i = 0; i < application_output->n_TLE; i++) {
        free(application_output->aTLE[i]);
        application_output->aTLE[i] = NULL;
    }

    for (int i = 0; i < application_output->n_EPHM; i++) {
        free(application_output->aEPHM[i]);
        application_output->aEPHM[i] = NULL;
    }

    for (int i = 0; i < application_output->n_TSPN; i++) {
        free(application_output->aTSPN[i]);
        application_output->aTSPN[i] = NULL;
    }

    application_output->n_TLE = 0;
    application_output->n_EPHM = 0;
    application_output->n_TSPN = 0;

    return OK;
}

