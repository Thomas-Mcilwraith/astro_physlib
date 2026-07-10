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
        LOG(INFO, "Loading TSPN outputs from program : %s %s", tlevision_input.tspn.source_program_id, tlevision_input.tspn.source_program_name);
        status = application_output_read_json(&tspn_output, execution_settings.working_directory, tlevision_input.tspn.source_program_id, tlevision_input.tspn.source_program_name);
        if (status != OK) {
            LOG(ERROR, "Failed to read program output: %s %s", tlevision_input.tspn.source_program_id, tlevision_input.tspn.source_program_name);
            return ERROR;
        }
    }

    // If the TLE is loaded from another program, load that output here.
    // Since TLEs must be source from the same program, assume first element
    if (tlevision_input.a_tle[0].source == TLE_SOURCE_PROGRAM) {
        LOG(INFO, "Loading aTLE outputs from program : %s %s", tlevision_input.tspn.source_program_id, tlevision_input.tspn.source_program_name);
        status = application_output_read_json(&a_tle_output, execution_settings.working_directory, tlevision_input.a_tle[0].source_program_id, tlevision_input.a_tle[0].source_program_name);
        if (status != OK) {
            LOG(ERROR, "Failed to read application output: %s %s",
                       tlevision_input.a_tle[0].source_program_id, tlevision_input.a_tle[0].source_program_name);
            return ERROR;
        }
    }

    // If the TLE is to be loaded from the catalog, load the catalog here.
    if (tlevision_input.a_tle[0].source == TLE_SOURCE_CATALOG) {
        LOG(INFO, "Loading SpaceTrack TLE catalogue");
        status = read_json(&tle_cat, database_tle_file);
        if (status != OK) {
            LOG(ERROR, "Failed to read JSON file: %s", database_tle_file);
            return ERROR;
        }
    }

    // Load the timespan
    LOG(INFO, "Loading timespan");
    status = application_inputs_tspn_load(&tspn, &tlevision_input.tspn, &tspn_output, &execution_settings, program_name);
    if (status != OK) {
        LOG(ERROR, "Failed to load timespan");
        return ERROR;
    }
    LOG(INFO, "Loaded: TSPN start: %f (%s %s)", tspn.values[0], tspn.name, tspn.units);
    LOG(INFO, "Loaded: TSPN end: %f (%s %s)", tspn.values[tspn.n_values - 1], tspn.name, tspn.units);
    LOG(INFO, "Loaded: TSPN n_points: %d", tspn.n_values);
    LOG(INFO, "Loaded: TSPN duration: %f days", tspn.values[tspn.n_values - 1] - tspn.values[0]);

    // Load the TLEs
    LOG(INFO, "Loading TLEs");
    status = tle_load(&a_tle, &n_tles, tlevision_input.a_tle, tlevision_input.n_tle, &a_tle_output, &execution_settings, tle_cat);
    if (status != OK) {
        LOG(ERROR, "Failed to load TLEs");
        return ERROR;
    }
    LOG(INFO, "Loaded %d TLEs successfully", n_tles);
    for (int i = 0; i < n_tles; i++) {
        LOG(INFO, "Loaded: aTLE[%d]: %s", i, a_tle[i].object_name);
        LOG(INFO, "Loaded: aTLE[%d]: %s", i, a_tle[i].tle_line1);
        LOG(INFO, "Loaded: aTLE[%d]: %s", i, a_tle[i].tle_line2);
    }

    // Generate the ephemeris with SGP4 model
    LOG(INFO, "Generating ephemeris with SGP4");
    status = 


    LOG(INFO, "Program complete: %s (%s)", program_name, execution_settings.run_title);

    // Deallocate memory
    // free(tles);
    // tlevision_inputs_free(&inputs);
    // application_output_free(&tspn_outputs);
    close_log();

    return OK;
}

