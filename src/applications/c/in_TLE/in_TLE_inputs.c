/*
 * in_TLE_inputs.c
 *
 * Author: Thomas McIlwraith
 * Date: 01/08/2026
 * 
 */

#include "in_TLE_inputs.h"

StatusCode in_TLE_inputs_read(
    // Outputs
    in_TLE_inputs_t *in_TLE,
    // Inputs
    const char *working_directory,
    const char *run_title) {

    // Program variables
    StatusCode status = OK;
    int len_filename = 0;
    cJSON *json = NULL;
    char filename[FILE_NAME_BUFFER_SIZE], filepath[FULL_PATH_BUFFER_SIZE];
    int n_input_TLEs = 0;

    if (in_TLE == NULL) {
        LOG(ERROR, "Bad pointer to tlevision_inputs_read");
        return ERROR;
    }

    len_filename = snprintf(filename, FILE_NAME_BUFFER_SIZE, "%s_inputs.json", run_title);
    if (len_filename >= FILE_NAME_BUFFER_SIZE) {
        LOG(ERROR, "Filename too long: %s", filename);
        return ERROR;
    }
    status = working_area_path(filepath, working_directory, INPUTS, filename, FULL_PATH_BUFFER_SIZE);
    if (status != OK) {
        LOG(ERROR, "Error constructing path to application output file: %s", filename);
        return status;
    }

    status = read_json(&json, filepath);
    if (status != OK) {
        LOG(ERROR, "Failed to read JSON file: %s", filepath);
        return ERROR;
    }

    // Assign the input variables
    cJSON *tle_array = cJSON_GetObjectItemCaseSensitive(json, "aTLE");
    if (!cJSON_IsArray(tle_array)) {
        LOG(ERROR, "aTLE is not an array in JSON file: %s", filepath);
        cJSON_Delete(json);
        return ERROR;
    }

    n_input_TLEs = cJSON_GetArraySize(tle_array);

    for (int i = 0; i < n_input_TLEs; i++) {

        // Resize the pointer array
        in_TLE->aTLE = realloc(in_TLE->aTLE, (i + 1) * sizeof(tle_t*));
        if (in_TLE->aTLE == NULL) {
            LOG(ERROR, "Failed to allocate memory for TLE");
            cJSON_Delete(json);
            return ERROR;
        }

        // Allocate the memory for this TLE
        in_TLE->aTLE[i] = malloc(sizeof(tle_t));
        if (in_TLE->aTLE[i] == NULL) {
            LOG(ERROR, "Failed to allocate memory for TLE");
            cJSON_Delete(json);
            return ERROR;
        }

        // Get the TLE data from the JSON
        cJSON *tle = cJSON_GetArrayItem(tle_array, i);
        if (tle == NULL) {
            LOG(ERROR, "Failed to get TLE from JSON");
            cJSON_Delete(json);
            return ERROR;
        }
    
        cJSON *object_id = cJSON_GetObjectItemCaseSensitive(tle, "object_id");
        if (cJSON_IsString(object_id) == false) {
            LOG(ERROR, "TLE object_id is missing from input file");
            cJSON_Delete(json);
            return ERROR;
        }
        in_TLE->aTLE[i]->object_id = strdup(cJSON_GetStringValue(object_id));

        cJSON *TLE_LINE1 = cJSON_GetObjectItemCaseSensitive(tle, "TLE_LINE1");
        if (cJSON_IsString(TLE_LINE1) == false) {
            LOG(ERROR, "TLE TLE_LINE1 is missing from input file");
            cJSON_Delete(json);
            return ERROR;
        }
        in_TLE->aTLE[i]->tle_line1 = strdup(cJSON_GetStringValue(TLE_LINE1));

        cJSON *TLE_LINE2 = cJSON_GetObjectItemCaseSensitive(tle, "TLE_LINE2");
        if (cJSON_IsString(TLE_LINE2) == false) {
            LOG(ERROR, "TLE TLE_LINE2 is missing from input file");
            cJSON_Delete(json);
            return ERROR;
        }
        in_TLE->aTLE[i]->tle_line2 = strdup(cJSON_GetStringValue(TLE_LINE2));

        in_TLE->n_TLE++;
    }

    cJSON_Delete(json);

    return OK;
}

