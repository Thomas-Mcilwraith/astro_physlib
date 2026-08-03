/*
 * in_TLE.c
 *
 * Author: Thomas McIlwraith
 * Date: 01/08/2026
 *
 * This program writes user-defined TLE elements to TLE files in the workspace
 * 
 */

#include "in_TLE_inputs.h"

int main(int argc, char *argv[]) {

    // Program variables
    StatusCode status = OK;
    execution_settings_t execution_settings;
    application_output_t output_file = {0};
    in_TLE_inputs_t in_TLE = {0};
    char *written_filename = NULL;

    // Parse command line, initialise log file
    status = parse_cmdline(&execution_settings, argc, argv);
    if (status != OK) {
        LOG(ERROR, "Failed to parse command line");
        return ERROR;
    }
    init_log(execution_settings.run_title, execution_settings.working_directory, execution_settings.program_name);

    // Load the inputs
    status = in_TLE_inputs_read(&in_TLE, execution_settings.working_directory, execution_settings.run_title);
    if (status != OK) {
        LOG(ERROR, "Failed to load inputs");
        return ERROR;
    }

    for (int i = 0; i < in_TLE.n_TLE; i++) {
        status = tle_write_single_json(&written_filename, in_TLE.aTLE[i], execution_settings.working_directory, execution_settings.run_title, i);
        if (status != OK) {
            LOG(ERROR, "Failed to write TLE file: %s", written_filename);
            return ERROR;
        }

        status = application_output_add_TLE(&output_file, written_filename);
        if (status != OK) {
            LOG(ERROR, "Failed to add TLE file to application output");
            return ERROR;
        }

        LOG(INFO, "TLE file written: %s", written_filename);
    }

    // Write the program output_file
    status = application_output_write(&output_file, execution_settings.working_directory, execution_settings.run_title);
    if (status != OK) {
        LOG(ERROR, "Failed to write output file");
        return ERROR;
    }

    LOG(INFO, "Program complete: %s (%s)", execution_settings.program_name, execution_settings.run_title);

    return status;
}
