/*
 * tlevision.c
 *
 * Author: Thomas McIlwraith
 * Date: 63/07/2026
 * 
 * Program for generating ephemerides from TLEs.
 * 
 */

#include "file-io/internal-products/application_output/application_output.h"
#include "file-io/internal-products/parameter-evolution-file/parameter_evolution_file.h"
#include "tlevision_interfaces.h"
#include "mathematics-library/numerical-methods/interpolation/interpolation.h"
#include "reference-systems/time-systems/time-formats/time_formats.h"
#include "utilities/logging/log/log.h"
#include "utilities/misc/parse-cmdline/parse_cmdline.h"
#include "utilities/misc/paths/paths.h"
#include "utilities/constants/constants.h"

int main(int argc, char *argv[]) {
    // Program configuration
    const char* program_name = "tlevision";
    ExecutionSettings execution_settings;
    tlevision_inputs_t inputs;
    application_output_t tspan_outputs;

    // Program variables
    StatusCode status = OK;
    // TODO: Retrieve from database
    const char* tle_file = "/home/admin/test_spacetrack_tle_cat.json";
    ParameterEvolutionFile pev_with_timespan;
    char pev_with_timespan_filename[FILE_NAME_BUFFER_SIZE];
    char pev_with_timespan_filepath[FULL_PATH_BUFFER_SIZE];
    double *jd_timespan = NULL;
    int len_jd_timespan = 0;
    datetime_t dt_start_time, dt_stop_time;
    double jd_start_time, jd_stop_time, jd_step_size;
    bool is_utc, found_timey_param;
    int timey_param_index;
    ParameterEvolution timespan;

    status = parse_cmdline(&execution_settings, argc, argv);
    if (status != OK) {
        LOG(ERROR, "Failed to parse command line");
        return ERROR;
    }
    init_log(execution_settings.run_title, execution_settings.working_directory, program_name);

    status = read_tlevision_inputs(&inputs, execution_settings.working_directory, execution_settings.run_title, program_name);
    if (status != OK) {
        LOG(ERROR, "Failed to read Inputs");
        return ERROR;
    }

    // Generate or read the timespan
    LOG(INFO, "Generating timespan");
    if (inputs.timespan_source == TLEVISION_INPUTS_TSPN_MANUAL) {

        // Read the timestamps
        status = iso8601_to_date(&dt_start_time, &is_utc, inputs.iso8601_start_time);
        status |= iso8601_to_date(&dt_stop_time, &is_utc, inputs.iso8601_stop_time);
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
        jd_step_size = inputs.step_size_seconds / SECONDS_PER_DAY;
        status = generate_linearly_spaced_array(&jd_timespan, &len_jd_timespan,
                                                jd_start_time, jd_stop_time, jd_step_size);
        if (status != OK) {
            LOG(ERROR, "Failed to generate timespan %f -> %f", jd_start_time, jd_stop_time);
            return ERROR;
        }

        // Construct the Parameter Evolution
        timespan.name = UTC;
        timespan.units = JD;
        timespan.n_values = len_jd_timespan;
        timespan.values = jd_timespan;

    } else if (inputs.timespan_source == TLEVISION_INPUTS_TSPN_PEV) {

        status = working_area_path(pev_with_timespan_filepath,
                execution_settings.working_directory, FILES, 
                inputs.pev_filename, FULL_PATH_BUFFER_SIZE);
        if (status != OK) {
            LOG(ERROR, "Failed to construct path for PEV file: %s", inputs.pev_filename);
            return ERROR;
        }

        status = read_parameter_evolution_file(&pev_with_timespan, pev_with_timespan_filepath);
        if (status != OK) {
            LOG(ERROR, "Failed to read PEV file to retreive timespan: %s", pev_with_timespan_filepath);
            return ERROR;
        }

        status = parameter_evolution_file_get_jd(&timespan, &pev_with_timespan);
        if (status != OK) {
            LOG(ERROR, "Failed to retrieve timespan from PEV file");
            return ERROR;
        }

    } else if (inputs.timespan_source == TLEVISION_INPUTS_TSPN_PROGRAM) {

        status = application_output_read(&tspan_outputs, execution_settings.working_directory,
                                         inputs.timespan_source_id, inputs.timespan_source_program);
        if (status != OK) {
            LOG(ERROR, "Failed to read application output");
            return ERROR;
        }

        if (tspan_outputs.n_TSPN != 1) {
            LOG(ERROR, "Expected 1 TSPAN file, found %d", tspan_outputs.n_TSPN);
            return ERROR;
        }

        status = working_area_path(pev_with_timespan_filepath,
                execution_settings.working_directory, FILES, 
                tspan_outputs.TSPN[0], FULL_PATH_BUFFER_SIZE);
        if (status != OK) {
            LOG(ERROR, "Failed to construct path for PEV file: %s", tspan_outputs.TSPN[0]);
            return ERROR;
        }

        status = read_parameter_evolution_file(&pev_with_timespan, pev_with_timespan_filepath);
        if (status != OK) {
            LOG(ERROR, "Failed to read PEV file to retreive timespan: %s", pev_with_timespan_filepath);
            return ERROR;
        }

        status = parameter_evolution_file_get_jd(&timespan, &pev_with_timespan);
        if (status != OK) {
            LOG(ERROR, "Failed to retrieve timespan from PEV file");
            return ERROR;
        }

    }

    // TODO: Compute UTC if time is provided in others
    if (strcmp(timespan.name, UTC) != 0) {
        LOG(WARNING, "Timespan is in %s", timespan.name);
        LOG(WARNING, "UTC == UT1 == TT will be assumed for the rest of this program");
        LOG(WARNING, "For improved accuracy, ensure source timespan is in UTC");
    }

    LOG(INFO, "Timespan with %d points loaded successfully", timespan.n_values);

    LOG(INFO, "Program complete: %s (%s)", program_name, execution_settings.run_title);
    tlevision_inputs_free(&inputs);
    application_output_free(&tspan_outputs);
    free(jd_timespan);
    close_log();
    return OK;

}
