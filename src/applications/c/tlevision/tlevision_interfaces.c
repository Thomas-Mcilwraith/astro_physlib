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
    tlevision_inputs_t *inputs,
    // Inputs
    const char *working_directory,
    const char *run_title,
    const char *program_name) {

    // Local variables
    cJSON *json;
    StatusCode status = OK;
    char filepath[1024];

    LOG(INFO, "Reading %s Inputs", program_name);

    // Construct the file path
    snprintf(filepath, sizeof(filepath), "%s/%s/%s_%s.json", working_directory,
             WORKDIR_INPUTS, run_title, program_name);

    // Read the JSON file
    status = read_json(&json, filepath);
    if (status != OK) {
        LOG(ERROR, "Failed to read JSON file: %s", filepath);
        return ERROR;
    }

    // Assign all the static values
    inputs->timespan_source = cJSON_GetObjectItemCaseSensitive(json, "timespan_source")->valueint;
    inputs->iso8601_start_time = cJSON_GetObjectItemCaseSensitive(json, "iso8601_start_time")->valuestring;
    inputs->iso8601_stop_time = cJSON_GetObjectItemCaseSensitive(json, "iso8601_stop_time")->valuestring;
    inputs->step_size_seconds = cJSON_GetObjectItemCaseSensitive(json, "step_size_seconds")->valuedouble;
    inputs->pev_filename = cJSON_GetObjectItemCaseSensitive(json, "pev_filename")->valuestring;
    inputs->tle_data_source = cJSON_GetObjectItemCaseSensitive(json, "tle_data_source")->valueint;
    inputs->tle_line_1 = cJSON_GetObjectItemCaseSensitive(json, "tle_line_1")->valuestring;
    inputs->tle_line_2 = cJSON_GetObjectItemCaseSensitive(json, "tle_line_2")->valuestring;
    inputs->timespan_source_id = cJSON_GetObjectItemCaseSensitive(json, "timespan_source_id")->valuestring;
    inputs->timespan_source_program = cJSON_GetObjectItemCaseSensitive(json, "timespan_source_program")->valuestring;
    inputs->tle_source_id = cJSON_GetObjectItemCaseSensitive(json, "tle_source_id")->valuestring;
    inputs->tle_source_program = cJSON_GetObjectItemCaseSensitive(json, "tle_source_program")->valuestring;

    // Assign the variable length arrays
    cJSON *object_ids = cJSON_GetObjectItemCaseSensitive(json, "object_ids");
    inputs->n_object_ids = cJSON_GetArraySize(object_ids);
    inputs->object_ids = malloc(inputs->n_object_ids * sizeof(char *));
    for (int i = 0; i < inputs->n_object_ids; i++) {
        inputs->object_ids[i] = cJSON_GetArrayItem(object_ids, i)->valuestring;
    }

    // Log all loaded values
    if (inputs->timespan_source == 0) {
        LOG(INFO, "Timespan Source: User inputs");
        LOG(INFO, "Loaded Input: ISO8601 Start Time: %s", inputs->iso8601_start_time);
        LOG(INFO, "Loaded Input: ISO8601 Stop Time: %s", inputs->iso8601_stop_time);
        LOG(INFO, "Loaded Input: Step Size Seconds: %f", inputs->step_size_seconds);
    } else if (inputs->timespan_source == 1) {
        LOG(INFO, "Timespan Source: PEV file");
        LOG(INFO, "Loaded Input: PEV Filename for timespan: %s", inputs->pev_filename);
    } else if (inputs->timespan_source == 2) {
        LOG(INFO, "Timespan Source: From %s %s", inputs->timespan_source_program, inputs->timespan_source_id);
    }

    if (inputs->tle_data_source == 0) {
        LOG(INFO, "TLE Data Source: User defined TLE strings");
        LOG(INFO, "Loaded Input: TLE Line 1: %s", inputs->tle_line_1);
        LOG(INFO, "Loaded Input: TLE Line 2: %s", inputs->tle_line_2);
    } else if (inputs->tle_data_source == 1) {
        LOG(INFO, "TLE Data Source: Search TLE catalogue for IDs");
        LOG(INFO, "Loaded Input: N Object IDs: %d", inputs->n_object_ids);
        for (int i = 0; i < inputs->n_object_ids; i++) {
            LOG(INFO, "Loaded Input: Object IDs[%d]: %s", i, inputs->object_ids[i]);
        }
    } else if (inputs->tle_data_source == 2) {
        LOG(INFO, "TLE Data Source: From %s %s", inputs->tle_source_program, inputs->tle_source_id);
    }

    return OK;
}

void tlevision_inputs_free(tlevision_inputs_t *options) {
    free(options->object_ids);
    return;
}
