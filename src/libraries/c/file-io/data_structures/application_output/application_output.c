/*
 * application_output.c
 *
 * Author: Thomas McIlwraith
 * Date: 18/04/2026
 */

#include "application_output.h"
#include "external/cjson/cJSON.h"

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

    // Allocate the memory for the arrays
    application_output->aTLE = malloc(application_output->n_TLE * sizeof(char *));
    application_output->aEPHM = malloc(application_output->n_EPHM * sizeof(char *));
    application_output->aTSPN = malloc(application_output->n_TSPN * sizeof(char *));
    if (application_output->aTLE == NULL || application_output->aEPHM == NULL || application_output->aTSPN == NULL) {
        LOG(ERROR, "Failed to allocate memory for application output arrays");
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

    // Free memory
    cJSON_Delete(json);

    return OK;
}

StatusCode application_output_write(
    // Inputs
    const application_output_t *application_output,
    const char* working_directory,
    const char* run_title) {

    // Local variables
    StatusCode status = OK;
    char filename[FILE_NAME_BUFFER_SIZE], filepath[FULL_PATH_BUFFER_SIZE];
    int len_filename = 0;
    cJSON *json = cJSON_CreateObject();
    cJSON *aTLE = NULL;
    cJSON *aEPHM = NULL;
    cJSON *aTSPN = NULL;

    if (application_output == NULL) {
        LOG(ERROR, "Bad pointer to application_output_write");
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

    // Add the aTLE array
    aTLE = cJSON_AddArrayToObject(json, "aTLE");
    if (aTLE == NULL) {
        LOG(ERROR, "Failed to add aTLE array to JSON object");
        return ERROR;
    }

    for (int i = 0; i < application_output->n_TLE; i++) {
        cJSON *aTLE_item = cJSON_CreateString(application_output->aTLE[i]);
        if (aTLE_item == NULL) {
            LOG(ERROR, "Failed to create aTLE item");
            return ERROR;
        }
        cJSON_AddItemToArray(aTLE, aTLE_item);
    }

    // Add the aEPHM array
    aEPHM = cJSON_AddArrayToObject(json, "aEPHM");
    if (aEPHM == NULL) {
        LOG(ERROR, "Failed to add aEPHM array to JSON object");
        return ERROR;
    }

    for (int i = 0; i < application_output->n_EPHM; i++) {
        cJSON *aEPHM_item = cJSON_CreateString(application_output->aEPHM[i]);
        if (aEPHM_item == NULL) {
            LOG(ERROR, "Failed to create aEPHM item");
            return ERROR;
        }
        cJSON_AddItemToArray(aEPHM, aEPHM_item);
    }

    // Add the aTSPN array
    aTSPN = cJSON_AddArrayToObject(json, "aTSPN");
    if (aTSPN == NULL) {
        LOG(ERROR, "Failed to add aTSPN array to JSON object");
        return ERROR;
    }

    for (int i = 0; i < application_output->n_TSPN; i++) {
        cJSON *aTSPN_item = cJSON_CreateString(application_output->aTSPN[i]);
        if (aTSPN_item == NULL) {
            LOG(ERROR, "Failed to create aTSPN item");
            return ERROR;
        }
        cJSON_AddItemToArray(aTSPN, aTSPN_item);
    }


    status = write_json(json, filepath);
    if (status != OK) {
        LOG(ERROR, "Failed to write JSON file: %s", filepath);
        return ERROR;
    }

    return OK;
}

StatusCode application_output_read_aTLE(
    // Outputs
    tle_t **aTLE,
    int *n_TLE,
    // Inputs
    const application_output_t *application_output,
    const char* working_directory
    ) {

    // Local variables
    StatusCode status = OK;
    cJSON *json = NULL;
    char filepath[FULL_PATH_BUFFER_SIZE];

    // Allocate memory for the TLE objects
    *aTLE = malloc(application_output->n_TLE * sizeof(tle_t));
    if (*aTLE == NULL) {
        LOG(ERROR, "Failed to allocate memory for TLE objects");
        return ERROR;
    }
    *n_TLE = application_output->n_TLE;

    // Load the TLE objects
    for (int i = 0; i < application_output->n_TLE; i++) {

        // Retrieve the absolute path for this file
        status = working_area_path(filepath, working_directory, FILES, application_output->aTLE[i], FULL_PATH_BUFFER_SIZE);
        if (status != OK) {
            LOG(ERROR, "Failed to construct path to TLE file: %s", application_output->aTLE[i]);
            return ERROR;
        }

        status = read_json(&json, filepath);
        if (status != OK) {
            LOG(ERROR, "Failed to read JSON file: %s", filepath);
            return ERROR;
        }

        status = tle_read_single_json(&(*aTLE)[i], json);
        if (status != OK) {
            LOG(ERROR, "Failed to read TLE file: %s", filepath);
            return ERROR;
        }
    }

    return OK;
}

StatusCode application_output_add_TLE(
    // Outputs
    application_output_t *application_output,
    // Inputs
    const char *filename) {

    // Local variables
    if (application_output == NULL) {
        LOG(ERROR, "Bad pointer to application_output_add_TLE");
        return ERROR;
    }

    // Resize the pointer array
    application_output->aTLE = realloc(application_output->aTLE, (application_output->n_TLE + 1) * sizeof(char *));
    if (application_output->aTLE == NULL) {
        LOG(ERROR, "Failed to reallocate aTLE array pointers");
        return ERROR;
    }

    // Allocate memeory for this entry
    application_output->aTLE[application_output->n_TLE] = malloc(sizeof(char *));
    if (application_output->aTLE[application_output->n_TLE] == NULL) {
        LOG(ERROR, "Failed to allocate memory for TLE");
        return ERROR;
    }

    application_output->aTLE[application_output->n_TLE] = strdup(filename);
    if (application_output->aTLE[application_output->n_TLE] == NULL) {
        LOG(ERROR, "Failed to set memory for TLE");
        return ERROR;
    }

    application_output->n_TLE++;

    return OK;
}

StatusCode application_output_add_EPHM(
    // Outputs
    application_output_t *application_output,
    // Inputs
    const char *filename) {

    // Local variables
    if (application_output == NULL) {
        LOG(ERROR, "Bad pointer to application_output_add_EPHM");
        return ERROR;
    }

    // if no elements are in the array, allocate memory for the first element
    if (application_output->n_EPHM == 0) {

        application_output->aEPHM = malloc(sizeof(char *));
        if (application_output->aEPHM == NULL) {
            LOG(ERROR, "Failed to allocate memory for EPHM");
            return ERROR;
        }

        application_output->aEPHM[0] = strdup(filename);
        if (application_output->aEPHM[0] == NULL) {
            LOG(ERROR, "Failed to set memory for EPHM");
            return ERROR;
        }

        application_output->n_EPHM++;

    // if there are elements in the array, realloc memory for the array
    } else {

        // otherwise, reallocate memory for the array
        application_output->aEPHM = realloc(application_output->aEPHM, (application_output->n_EPHM + 1) * sizeof(char *));
        if (application_output->aEPHM == NULL) {
            LOG(ERROR, "Failed to reallocate memory for EPHM");
            return ERROR;
        }

        application_output->aEPHM[application_output->n_EPHM] = strdup(filename);
        if (application_output->aEPHM[application_output->n_EPHM] == NULL) {
            LOG(ERROR, "Failed to set memory for EPHM");
            return ERROR;
        }

        application_output->n_EPHM++;
    }

    return OK;
}

StatusCode application_output_add_TSPN(
    // Outputs
    application_output_t *application_output,
    // Inputs
    const char *filename) {

    // Local variables
    if (application_output == NULL) {
        LOG(ERROR, "Bad pointer to application_output_add_TSPN");
        return ERROR;
    }

    // if no elements are in the array, allocate memory for the first element
    if (application_output->n_TSPN == 0) {

        application_output->aTSPN = malloc(sizeof(char *));
        if (application_output->aTSPN == NULL) {
            LOG(ERROR, "Failed to allocate memory for TSPN");
            return ERROR;
        }

        application_output->aTSPN[0] = strdup(filename);
        if (application_output->aTSPN[0] == NULL) {
            LOG(ERROR, "Failed to set memory for TSPN");
            return ERROR;
        }

        application_output->n_TSPN++;

    // if there are elements in the array, realloc memory for the array
    } else {

        // otherwise, reallocate memory for the array
        application_output->aTSPN = realloc(application_output->aTSPN, (application_output->n_TSPN + 1) * sizeof(char *));
        if (application_output->aTSPN == NULL) {
            LOG(ERROR, "Failed to reallocate memory for TSPN");
            return ERROR;
        }

        application_output->aTSPN[application_output->n_TSPN] = strdup(filename);
        if (application_output->aTSPN[application_output->n_TSPN] == NULL) {
            LOG(ERROR, "Failed to set memory for TSPN");
            return ERROR;
        }

        application_output->n_TSPN++;
    }

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

