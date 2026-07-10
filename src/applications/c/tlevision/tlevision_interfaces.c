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
#include "file-io/data_structures/application_input_tle/application_input_tle.h"

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

    // Printouts
    LOG(INFO, "Loaded Input (wgs_model): %d", inputs->wgs_model);
    LOG(INFO, "Loaded Input (tspn source): %d", inputs->tspn.source);
    if (inputs->tspn.source == TSPN_SOURCE_PROGRAM) {
        LOG(INFO, "Loaded Input (tspn source_program_id): %s", inputs->tspn.source_program_id);
        LOG(INFO, "Loaded Input (tspn source_program_name): %s", inputs->tspn.source_program_name);
    } else if (inputs->tspn.source == TSPN_SOURCE_USER) {
        LOG(INFO, "Loaded Input (tspn iso8601_start_time): %s", inputs->tspn.iso8601_start_time);
        LOG(INFO, "Loaded Input (tspn iso8601_stop_time): %s", inputs->tspn.iso8601_stop_time);
        LOG(INFO, "Loaded Input (tspn step_size_seconds): %f", inputs->tspn.step_size_seconds);
    }

    LOG(INFO, "Loaded Input (a_tle source): %d", inputs->a_tle[0].source);
    for (int i = 0; i < inputs->n_tle; i++) {
        if (inputs->a_tle[i].source == TLE_SOURCE_PROGRAM) {
            LOG(INFO, "Loaded Input (a_tle[%d] source_program_id): %s", i, inputs->a_tle[i].source_program_id);
            LOG(INFO, "Loaded Input (a_tle[%d] source_program_name): %s", i, inputs->a_tle[i].source_program_name);
        } else if (inputs->a_tle[i].source == TLE_SOURCE_USER) {
            LOG(INFO, "Loaded Input (a_tle[%d] object_name): %s", i, inputs->a_tle[i].object_name);
            LOG(INFO, "Loaded Input (a_tle[%d] line_1): %s", i, inputs->a_tle[i].tle_line_1);
            LOG(INFO, "Loaded Input (a_tle[%d] line_2): %s", i, inputs->a_tle[i].tle_line_2);
        } else if (inputs->a_tle[i].source == TLE_SOURCE_CATALOG) {
            LOG(INFO, "Loaded Input (a_tle[%d] object_cospar_id): %s", i, inputs->a_tle[i].object_cospar_id);
        }
    }

    return OK;
}
