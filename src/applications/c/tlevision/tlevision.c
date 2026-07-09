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
#include "tlevision_load_timespan.h"
#include "file-io/internal-products/application_output/application_output.h"
#include "file-io/internal-products/parameter-evolution-file/parameter_evolution_file.h"
#include "mathematics-library/numerical-methods/interpolation/interpolation.h"
#include "reference-systems/time-systems/time-formats/time_formats.h"
#include "utilities/logging/log/log.h"
#include "utilities/misc/parse-cmdline/parse_cmdline.h"
#include "utilities/misc/paths/paths.h"
#include "utilities/constants/constants.h"

int main(int argc, char *argv[]) {
    // Program configuration
    const char* program_name = "tlevision";
    ExecutionSettings execution_settings = {0};
    tlevision_inputs_t inputs = {0};
    application_output_t tspn_outputs = {0};

    // Program variables
    StatusCode status = OK;
    // TODO: Retrieve from database
    const char* tle_file = "/home/admin/test_spacetrack_tle_cat.json";
    char pev_with_timespan_filename[FILE_NAME_BUFFER_SIZE];
    char pev_with_timespan_filepath[FULL_PATH_BUFFER_SIZE];
    ParameterEvolutionFile pev_with_timespan = {0};
    ParameterEvolution timespan = {0};

    // Parse command line, initialise log file
    status = parse_cmdline(&execution_settings, argc, argv);
    if (status != OK) {
        LOG(ERROR, "Failed to parse command line");
        return ERROR;
    }
    init_log(execution_settings.run_title, execution_settings.working_directory, program_name);

    // Read the input file
    status = read_tlevision_inputs(&inputs, execution_settings.working_directory, execution_settings.run_title, program_name);
    if (status != OK) {
        LOG(ERROR, "Failed to read Inputs");
        return ERROR;
    }

    // If the timespan is taken from another programs outputs, load the PEV here
    if (inputs.timespan_source == TLEVISION_INPUTS_TSPN_PROGRAM) {
        status = application_output_read(&tspn_outputs, execution_settings.working_directory, inputs.timespan_source_id, inputs.timespan_source_program);
        if (status != OK) {
            LOG(ERROR, "Failed to read application output: %s %s",
                       inputs.timespan_source_id, inputs.timespan_source_program);
            return ERROR;
        }

        if (tspn_outputs.n_TSPN != 1) {
            LOG(ERROR, "Expected 1 TSPAN file, found %d", tspn_outputs.n_TSPN);
            return ERROR;
        }

        status = working_area_path(pev_with_timespan_filepath, execution_settings.working_directory, FILES, tspn_outputs.TSPN[0], FULL_PATH_BUFFER_SIZE);
        if (status != OK) {
            LOG(ERROR, "Failed to construct path for PEV file: %s", tspn_outputs.TSPN[0]);
            return ERROR;
        }

        status = read_parameter_evolution_file(&pev_with_timespan, pev_with_timespan_filepath);
        if (status != OK) {
            LOG(ERROR, "Failed to read PEV file to retreive timespan: %s", pev_with_timespan_filepath);
            return ERROR;
        }

    // If the timespan is taken from a specific PEV file, load the PEV here
    } else if (inputs.timespan_source == TLEVISION_INPUTS_TSPN_PEV) {

        status = working_area_path(pev_with_timespan_filepath, execution_settings.working_directory, FILES, inputs.pev_filename, FULL_PATH_BUFFER_SIZE);
        if (status != OK) {
            LOG(ERROR, "Failed to construct path for PEV file: %s", inputs.pev_filename);
            return ERROR;
        }

        status = read_parameter_evolution_file(&pev_with_timespan, pev_with_timespan_filepath);
        if (status != OK) {
            LOG(ERROR, "Failed to read PEV file to retreive timespan: %s", pev_with_timespan_filepath);
            return ERROR;
        }

    }

    // Load the timespan
    LOG(INFO, "Generating timespan");
    status = load_timespan(&timespan, &pev_with_timespan, &inputs, &execution_settings);
    if (status != OK) {
        LOG(ERROR, "Failed to load timespan");
        return ERROR;
    }

    LOG(INFO, "Timespan with %d points loaded successfully", timespan.n_values);

    LOG(INFO, "Program complete: %s (%s)", program_name, execution_settings.run_title);
    tlevision_inputs_free(&inputs);
    application_output_free(&tspn_outputs);
    close_log();
    return OK;
}

