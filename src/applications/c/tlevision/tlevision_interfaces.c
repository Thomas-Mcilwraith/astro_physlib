/*
 * tlevision_interfaces.c
 *
 * Author: Thomas McIlwraith
 * Date: 63/07/2026
 * 
 * Interface structures and functions for tlevision.
 * 
 */

#include "tlevision_interfaces.h"

StatusCode read_tlevision_inputs(
    // Outputs
    tlevision_input_t *inputs,
    // Inputs
    const char *working_directory,
    const char *run_title,
    const char *program_name) {

    // Local variables
    cJSON *json;
    StatusCode status = OK;
    char filepath[FULL_PATH_BUFFER_SIZE], filename[FILE_NAME_BUFFER_SIZE];

    LOG(INFO, "Reading %s Inputs", program_name);

    // Construct the file name
    snprintf(filename, sizeof(filename), "%s_%s.json", run_title, program_name);
    // Construct the file path
    working_area_path(filepath, working_directory, INPUTS, filename, FULL_PATH_BUFFER_SIZE);

    // Read the JSON file
    status = read_json(&json, filepath);
    if (status != OK) {
        LOG(ERROR, "Failed to read JSON file: %s", filepath);
        return ERROR;
    }

    // Read the tlevision inputs
    const cJSON *tlevision = cJSON_GetObjectItemCaseSensitive(json, "tlevision");
    if (!cJSON_IsObject(tlevision)) {
        LOG(ERROR, "Malformed Inputs: tlevision");
        return ERROR;
    }

    // Read the WGS model
    const cJSON *wgs_model = cJSON_GetObjectItemCaseSensitive(tlevision, "wgs_model");
    if (!cJSON_IsNumber(wgs_model)) {
        LOG(ERROR, "Incorrect type in TSPN: wgs_model");
        return ERROR;
    }
    inputs->wgs_model = wgs_model->valueint;

    // Read the tspn
    const cJSON *tspn = cJSON_GetObjectItemCaseSensitive(json, "tspn");
    if (!cJSON_IsObject(tspn)) {
        LOG(ERROR, "Malformed Inputs: tspn");
        return ERROR;
    }
    status = application_input_tspn_read_json(&inputs->tspn, tspn);;
    if (status != OK) {
        LOG(ERROR, "Failed to read Inputs: tspn");
        return ERROR;
    }

    // Get the number of TLEs
    const cJSON *a_tle = cJSON_GetObjectItemCaseSensitive(json, "a_tle");
    if (!cJSON_IsArray(a_tle)) {
        LOG(ERROR, "Malformed Inputs: a_tle");
        return ERROR;
    }
    inputs->n_tle = cJSON_GetArraySize(a_tle);

    // Allocate memory for the TLEs
    inputs->a_tle = malloc(inputs->n_tle * sizeof(application_input_tle_t));
    if (inputs->a_tle == NULL) {
        LOG(ERROR, "Failed to allocate memory for TLEs");
        return ERROR;
    }

    // Read the TLEs
    for (int i = 0; i < inputs->n_tle; i++) {
        const cJSON *tle = cJSON_GetArrayItem(a_tle, i);
        if (!cJSON_IsObject(tle)) {
            LOG(ERROR, "Malformed Inputs: a_tle[%d]", i);
            return ERROR;
        }
        status = application_input_tle_read_json(&inputs->a_tle[i], tle);
        if (status != OK) {
            LOG(ERROR, "Failed to read Inputs: a_tle[%d]", i);
            return ERROR;
        }
    }

    return OK;
}
