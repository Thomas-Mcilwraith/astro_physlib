/*
 * tlevision.c
 *
 * Author: Thomas McIlwraith
 * Date: 63/07/2026
 * 
 * Program for generating ephemerides from TLEs.
 * 
 */

#include "file-io/internal-products/parameter-evolution-file/parameter_evolution_file.h"
#include "file-io/internal-products/read-json/read_json.h"
#include "file-io/data_structures/tle/tle.h"
#include "tlevision_interfaces.h"
#include "utilities/misc/paths/paths.h"

int main(int argc, char *argv[]) {
    // Program configuration
    const char* program_name = "tlevision";
    ExecutionSettings execution_settings = {0};
    tlevision_input_t tlevision_input = {0};
    application_output_t tlevision_output = {0};
    application_output_t tspn_output = {0};
    application_output_t a_tle_output = {0};

    // Program variables
    StatusCode status = OK;
    ParameterEvolution tspn = {0};
    tle_t *a_tle = {0};
    int n_tles = 0;
    const char* database_tle_file = "/home/admin/test_spacetrack_tle_cat.json";  // TODO: Retrieve from database
    cJSON *tle_cat;

    // Parse command line, initialise log file
    status = parse_cmdline(&execution_settings, argc, argv);
    if (status != OK) {
        LOG(ERROR, "Failed to parse command line");
        return ERROR;
    }
    init_log(execution_settings.run_title, execution_settings.working_directory, program_name);

    // Read the input file
    status = read_tlevision_inputs(&tlevision_input, execution_settings.working_directory, execution_settings.run_title, program_name);
    if (status != OK) {
        LOG(ERROR, "Failed to read Inputs");
        return ERROR;
    }

    // If timespan is loaded from another program. Load that output here.
    if (tlevision_input.tspn.source == TSPN_SOURCE_PROGRAM) {
        status = application_output_read_json(&tspn_output, execution_settings.working_directory, tlevision_input.tspn.source_program_id, tlevision_input.tspn.source_program_name);
        if (status != OK) {
            LOG(ERROR, "Failed to read application output: %s %s",
                       tlevision_input.tspn.source_program_id, tlevision_input.tspn.source_program_name);
            return ERROR;
        }
    }

    // If the TLE is loaded from another program, load that output here.
    // Since TLEs must be source from the same program, assume first element
    if (tlevision_input.a_tle[0].source == TLE_SOURCE_PROGRAM) {
        status = application_output_read_json(&a_tle_output, execution_settings.working_directory, tlevision_input.a_tle[0].source_program_id, tlevision_input.a_tle[0].source_program_name);
        if (status != OK) {
            LOG(ERROR, "Failed to read application output: %s %s",
                       tlevision_input.a_tle[0].source_program_id, tlevision_input.a_tle[0].source_program_name);
            return ERROR;
        }
    }

    // If the TLE is to be loaded from the catalog, load the catalog here.
    if (tlevision_input.a_tle[0].source == TLE_SOURCE_CATALOG) {
        status = read_json(&tle_cat, database_tle_file);
        if (status != OK) {
            LOG(ERROR, "Failed to read JSON file: %s", database_tle_file);
            return ERROR;
        }
    }

    // Load the timespan
    status = application_inputs_tspn_load(&tspn, &tlevision_input.tspn, &tspn_output, &execution_settings, program_name);
    if (status != OK) {
        LOG(ERROR, "Failed to load timespan");
        return ERROR;
    }

    // Load the TLEs
    status = tle_load(&a_tle, &n_tles, tlevision_input.a_tle, tlevision_input.n_tle, &a_tle_output, &execution_settings, tle_cat);
    if (status != OK) {
        LOG(ERROR, "Failed to load TLEs");
        return ERROR;
    }

    // // If the timespan is taken from another programs outputs, load the PEV here
    // if (inputs.timespan_source == TLEVISION_INPUTS_TSPN_PROGRAM) {
    //     status = application_output_read(&tspn_outputs, execution_settings.working_directory, inputs.timespan_source_id, inputs.timespan_source_program);
    //     if (status != OK) {
    //         LOG(ERROR, "Failed to read application output: %s %s",
    //                    inputs.timespan_source_id, inputs.timespan_source_program);
    //         return ERROR;
    //     }
    //
    //     if (tspn_outputs.n_TSPN != 1) {
    //         LOG(ERROR, "Expected 1 TSPAN file, found %d", tspn_outputs.n_TSPN);
    //         return ERROR;
    //     }
    //
    //     status = working_area_path(pev_with_timespan_filepath, execution_settings.working_directory, FILES, tspn_outputs.TSPN[0], FULL_PATH_BUFFER_SIZE);
    //     if (status != OK) {
    //         LOG(ERROR, "Failed to construct path for PEV file: %s", tspn_outputs.TSPN[0]);
    //         return ERROR;
    //     }
    //
    //     status = read_parameter_evolution_file(&pev_with_timespan, pev_with_timespan_filepath);
    //     if (status != OK) {
    //         LOG(ERROR, "Failed to read PEV file to retreive timespan: %s", pev_with_timespan_filepath);
    //         return ERROR;
    //     }
    //
    // // If the timespan is taken from a specific PEV file, load the PEV here
    // } else if (inputs.timespan_source == TLEVISION_INPUTS_TSPN_PEV) {
    //
    //     status = working_area_path(pev_with_timespan_filepath, execution_settings.working_directory, FILES, inputs.pev_filename, FULL_PATH_BUFFER_SIZE);
    //     if (status != OK) {
    //         LOG(ERROR, "Failed to construct path for PEV file: %s", inputs.pev_filename);
    //         return ERROR;
    //     }
    //
    //     status = read_parameter_evolution_file(&pev_with_timespan, pev_with_timespan_filepath);
    //     if (status != OK) {
    //         LOG(ERROR, "Failed to read PEV file to retreive timespan: %s", pev_with_timespan_filepath);
    //         return ERROR;
    //     }
    //
    // }
    //
    // // Load the timespan
    // LOG(INFO, "Generating timespan");
    // status = load_timespan(&timespan, &pev_with_timespan, &inputs, &execution_settings);
    // if (status != OK) {
    //     LOG(ERROR, "Failed to load timespan");
    //     return ERROR;
    // }
    //
    // LOG(INFO, "Timespan with %d points loaded successfully", timespan.n_values);

    // // Load all the TLEs into TLE objects
    // // Manual input only supports one input TLE
    // if (inputs.tle_data_source == TSPN_SOURCE_PROGRAM) {
    //     n_tles = 1;
    //     tles = malloc(sizeof(TLE));
    //     if (tles == NULL) {
    //         LOG(ERROR, "Failed to allocate memory for TLE");
    //         return ERROR;
    //     }
    //     // TODO: Add constants model choice to this call and program inputs
    //     parseLines(tles, inputs.tle_line_1, inputs.tle_line_2);
    //
    // // If a list of TLE IDs is provided, the catalog must be loaded and seached
    // } else if (inputs.tle_data_source == TLEVISION_INPUTS_TLE_ID) {
    //     // TODO: Load the catalog
    //     // TODO: Search the catalog for the TLE IDs
    //     // TODO: Add those TLEs to tles
    //
    // // If TLEs are provided by another program, load every item from that catalog
    // } else if (inputs.tle_data_source == TLEVISION_INPUTS_TLE_PROGRAM) {
    //     // TODO: Load the ouputs from the other program (reduced catalog)
    //     // TODO: Add those TLEs to tles
    // }

    LOG(INFO, "Loaded %d TLEs successfully", n_tles);

    LOG(INFO, "Program complete: %s (%s)", program_name, execution_settings.run_title);

    // Deallocate memory
    // free(tles);
    // tlevision_inputs_free(&inputs);
    // application_output_free(&tspn_outputs);
    close_log();

    return OK;
}

