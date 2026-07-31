/*
 * tlevision_inputs.c
 *
 * Author: Thomas McIlwraith
 * Date: 31/07/2026
 * 
 */

#include "tlevision_inputs.h"

StatusCode tlevision_inputs_read(
    // Outputs
    tlevision_inputs_t *inputs,
    // Inputs
    const char *working_directory,
    const char *run_title
    ) {

    // Program variables
    StatusCode status = OK;
    int len_filename = 0;
    cJSON *json = NULL;
    char filename[FILE_NAME_BUFFER_SIZE], filepath[FULL_PATH_BUFFER_SIZE];

    if (inputs == NULL) {
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
    cJSON *wgs_model = cJSON_GetObjectItemCaseSensitive(json, "wgs_model");
    if (cJSON_IsString(wgs_model) == false) {
        LOG(ERROR, "WGS model is missing from input file");
        cJSON_Delete(json);
        return ERROR;
    }
    inputs->wgs_model = strdup(cJSON_GetStringValue(wgs_model));

    cJSON_Delete(json);

    return OK;
}
