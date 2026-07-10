/*
 * application_input_tspn.c
 *
 * Author: Thomas McIlwraith
 * Date: 18/04/2026
 */

#include "application_input_tspn.h"

StatusCode application_input_tspn_read_json(
    // Outputs
    application_input_tspn_t *tspn_input,
    // Inputs
    const cJSON *tspn_json) {

    // Local variables

    // Assign all the static values
    const cJSON *source = cJSON_GetObjectItemCaseSensitive(tspn_json, "source");
    if (!cJSON_IsNumber(source)) {
        LOG(ERROR, "Incorrect type in TSPN: source");
        return ERROR;
    }
    tspn_input->source = source->valueint;

    // Case source == 0 - Read the user defined inputs
    if (tspn_input->source == TSPN_SOURCE_USER) {

        const cJSON *iso8601_start_time = cJSON_GetObjectItemCaseSensitive(tspn_json, "iso8601_start_time");
        if (!cJSON_IsString(iso8601_start_time)) {
            LOG(ERROR, "Incorrect type in TSPN: iso8601_start_time");
            return ERROR;
        }
        tspn_input->iso8601_start_time = strdup(iso8601_start_time->valuestring);

        const cJSON *iso8601_stop_time = cJSON_GetObjectItemCaseSensitive(tspn_json, "iso8601_stop_time");
        if (!cJSON_IsString(iso8601_stop_time)) {
            LOG(ERROR, "Incorrect type in TSPN: iso8601_stop_time");
            return ERROR;
        }
        tspn_input->iso8601_stop_time = strdup(iso8601_stop_time->valuestring);

        const cJSON *step_size_seconds = cJSON_GetObjectItemCaseSensitive(tspn_json, "step_size_seconds");
        if (!cJSON_IsNumber(step_size_seconds)) {
            LOG(ERROR, "Incorrect type in TSPN: step_size_seconds");
            return ERROR;
        }
        tspn_input->step_size_seconds = step_size_seconds->valueint;

    // Case source == 1 - Read the program outputs
    } else if (tspn_input->source == TSPN_SOURCE_PROGRAM) {

        const cJSON *source_program_id = cJSON_GetObjectItemCaseSensitive(tspn_json, "source_program_id");
        if (!cJSON_IsString(source_program_id)) {
            LOG(ERROR, "Incorrect type in TSPN: source_program_id");
            return ERROR;
        }
        tspn_input->source_program_id = strdup(source_program_id->valuestring);

        const cJSON *source_program_name = cJSON_GetObjectItemCaseSensitive(tspn_json, "source_program_name");
        if (!cJSON_IsString(source_program_name)) {
            LOG(ERROR, "Incorrect type in TSPN: source_program_name");
            return ERROR;
        }
        tspn_input->source_program_name = strdup(source_program_name->valuestring);

    } else {
        LOG(ERROR, "Incorrect value in TSPN: source = %d", tspn_input->source);
        return ERROR;
    }

    return OK;
}

StatusCode application_inputs_tspn_load(
        // Outputs
        ParameterEvolution *tspn,
        // Inputs
        const application_input_tspn_t *tspn_inputs,
        const application_output_t *tspn_outputs,
        const ExecutionSettings *execution_settings,
        const char *program_name
        ) {

    // Local variables
    StatusCode status = OK;
    datetime_t dt_start_time, dt_stop_time;
    double jd_start_time, jd_stop_time, jd_step_size;
    bool is_utc;
    double *jd_timespan = NULL;
    int len_jd_timespan = 0;
    char pev_with_timespan_filepath[FULL_PATH_BUFFER_SIZE];
    char pev_with_timespan_filename[FILE_NAME_BUFFER_SIZE];
    ParameterEvolutionFile pev_with_timespan = {0};

    // If user inputs are taken, read the strings and generate the timespan
    if (tspn_inputs->source == TSPN_SOURCE_USER) {

        // Read the timestamps
        status = iso8601_to_date(&dt_start_time, &is_utc, tspn_inputs->iso8601_start_time);
        status |= iso8601_to_date(&dt_stop_time, &is_utc, tspn_inputs->iso8601_stop_time);
        if (status != OK) {
            LOG(ERROR, "Failed to parse start/stop times");
            return ERROR;
        }

        // Convert to JD
        status = date_to_jd(&jd_start_time, dt_start_time);
        status |= date_to_jd(&jd_stop_time, dt_stop_time);
        if (status != OK) {
            LOG(ERROR, "Failed to convert start/stop times to JD");
            return ERROR;
        }

        // Compute the timespan
        jd_step_size = tspn_inputs->step_size_seconds / SECONDS_PER_DAY;
        status = generate_linearly_spaced_array(&jd_timespan, &len_jd_timespan,
                                                jd_start_time, jd_stop_time, jd_step_size);
        if (status != OK) {
            LOG(ERROR, "Failed to generate timespan %f -> %f", jd_start_time, jd_stop_time);
            return ERROR;
        }

        // Construct the Parameter Evolution
        tspn->name = UTC;
        tspn->units = JD;
        tspn->n_values = len_jd_timespan;
        tspn->values = jd_timespan;

    // If the timespan is taken from another programs outputs,
    } else if (tspn_inputs->source == TSPN_SOURCE_PROGRAM) {

        if (tspn_outputs->n_TSPN != 1) {
            LOG(ERROR, "Expected 1 TSPN file, found %d", tspn_outputs->n_TSPN);
            return ERROR;
        }

        // Construct the file name
        snprintf(pev_with_timespan_filename, sizeof(pev_with_timespan_filename), "%s_%s.json", execution_settings->run_title, program_name);

        // Construct the file path
        status = working_area_path(pev_with_timespan_filepath, execution_settings->working_directory, FILES, pev_with_timespan_filename, FULL_PATH_BUFFER_SIZE);
        if (status != OK) {
            LOG(ERROR, "Failed to construct path for PEV file: %s", pev_with_timespan_filename);
            return ERROR;
        }

        // Read the PEV file
        status = read_parameter_evolution_file(&pev_with_timespan, pev_with_timespan_filepath);
        if (status != OK) {
            LOG(ERROR, "Failed to read PEV file to retreive timespan: %s", pev_with_timespan_filepath);
            return ERROR;
        }

        // Load the time column from the PEV file
        status = parameter_evolution_file_get_jd(tspn, &pev_with_timespan);
        if (status != OK) {
            LOG(ERROR, "Failed to retrieve timespan from PEV file");
            return ERROR;
        }

    }

    return OK;
}

void application_input_tspn_free(application_input_tspn_t *tspn_input) {
    if (tspn_input == NULL) {
        return;
    }

    free(tspn_input->iso8601_start_time);
    free(tspn_input->iso8601_stop_time);
    free(tspn_input->source_program_id);
    free(tspn_input->source_program_name);
}
