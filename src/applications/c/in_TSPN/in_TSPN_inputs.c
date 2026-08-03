/*
 * in_TSPN_inputs.c
 *
 * Author: Thomas McIlwraith
 * Date: 03/08/2026
 * 
 */

#include "in_TSPN_inputs.h"

StatusCode in_TSPN_inputs_read(
    // Outputs
    in_TSPN_inputs_t *in_TSPN,
    // Inputs
    const char *working_directory,
    const char *run_title) {

    // Program variables
    StatusCode status = OK;
    int len_filename = 0;
    cJSON *json = NULL;
    char filename[FILE_NAME_BUFFER_SIZE], filepath[FULL_PATH_BUFFER_SIZE];

    if (in_TSPN == NULL) {
        LOG(ERROR, "Bad pointer to in_TSPN_inputs_read");
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
    cJSON *start_time_iso = cJSON_GetObjectItemCaseSensitive(json, "start_time_iso");
    if (cJSON_IsString(start_time_iso) == false) {
        LOG(ERROR, "start_time_iso is missing from input file");
        cJSON_Delete(json);
        return ERROR;
    }
    in_TSPN->start_time_iso = strdup(cJSON_GetStringValue(start_time_iso));

    cJSON *end_time_iso = cJSON_GetObjectItemCaseSensitive(json, "end_time_iso");
    if (cJSON_IsString(end_time_iso) == false) {
        LOG(ERROR, "end_time_iso is missing from input file");
        cJSON_Delete(json);
        return ERROR;
    }
    in_TSPN->end_time_iso = strdup(cJSON_GetStringValue(end_time_iso));

    cJSON *stepsize_seconds = cJSON_GetObjectItemCaseSensitive(json, "stepsize_seconds");
    if (cJSON_IsNumber(stepsize_seconds) == false) {
        LOG(ERROR, "stepsize_seconds is missing from input file");
        cJSON_Delete(json);
        return ERROR;
    }
    in_TSPN->stepsize_seconds = cJSON_GetNumberValue(stepsize_seconds);

    cJSON_Delete(json);

    return OK;
}
