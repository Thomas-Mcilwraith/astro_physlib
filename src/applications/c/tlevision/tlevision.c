/*
 * tlevision.c
 *
 * Author: Thomas McIlwraith
 * Date: 63/07/2026
 * 
 * Program for generating ephemerides from TLEs.
 * 
 */

#include "utilities/logging/log/log.h"
#include "utilities/misc/parse-cmdline/parse_cmdline.h"
#include "tlevision_interfaces.h"

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

    LOG(INFO, "Program complete: %s %s", program_name, execution_settings.run_title);
    tlevision_inputs_free(&inputs);
    close_log();
    return OK;

};
