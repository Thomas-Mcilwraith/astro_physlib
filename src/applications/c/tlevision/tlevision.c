/*
 * tlevision.c
 *
 * Author: Thomas McIlwraith
 * Date: 63/07/2026
 * 
 * Program for generating ephemerides from TLEs.
 * 
 */

#include "tlevision_interfaces.h"
#include "mathematics-library/numerical-methods/interpolation/interpolation.h"
#include "utilities/logging/log/log.h"
#include "utilities/misc/parse-cmdline/parse_cmdline.h"
#include "utilities/constants/constants.h"

int main(int argc, char *argv[]) {
    // Program configuration
    const char* program_name = "tlevision";
    ExecutionSettings execution_settings;
    tlevision_inputs_t inputs;
    tlevision_outputs_t outputs;

    // Program variables
    StatusCode status = OK;
    // TODO: Retrieve from database
    const char* tle_file = "/home/admin/test_spacetrack_tle_cat.json";
    double *jd_timespan = NULL;
    int len_jd_timespan = 0;
    double jd_start_time, jd_stop_time, jd_step_size;

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
    if (inputs.timespan_source == TLEVISION_INPUTS_TSPN_MANUAL) {
        jd_step_size = inputs.step_size_seconds / SECONDS_PER_DAY;
        status = generate_linearly_spaced_array(&jd_timespan, &len_jd_timespan,
                                                jd_start_time, jd_stop_time, jd_step_size);
    } else if (inputs.timespan_source == TLEVISION_INPUTS_TSPN_PEV) {
        // TODO: Generate timespan
    } else if (inputs.timespan_source == TLEVISION_INPUTS_TSPN_PROGRAM) {
        // TODO: Generate timespan
    }

    LOG(INFO, "Program complete: %s (%s)", program_name, execution_settings.run_title);
    tlevision_inputs_free(&inputs);
    free(jd_timespan);
    close_log();
    return OK;

};
