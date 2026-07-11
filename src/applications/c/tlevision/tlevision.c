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
#include "tlevision/tlevision_generate_arc.h"
#include "tlevision_interfaces.h"
#include "utilities/misc/paths/paths.h"

int main(int argc, char *argv[]) {
    // Outputs
    application_output_t tlevision_output = {0};
    ParameterEvolutionFile *ephm_output = {0};
    int n_ephm_output = 0;
    // Inputs
    tlevision_input_t tlevision_input = {0};
    const char* program_name = "tlevision";
    ExecutionSettings execution_settings = {0};
    application_output_t tspn_output = {0};
    application_output_t a_tle_output = {0};

    // Program variables
    StatusCode status = OK;
    const char* database_tle_file = "/home/admin/test_spacetrack_tle_cat.json";  // TODO: Retrieve from database
    cJSON *tle_cat;
    ParameterEvolution tspn = {0};
    tle_t *a_tle = {0};
    int n_tles = 0;

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
            LOG(ERROR, "Failed to read program output: %s %s", tlevision_input.tspn.source_program_id, tlevision_input.tspn.source_program_name);
            return ERROR;
        }
    }

    // If the TLE is loaded from another program, load that output here.
    // Since TLEs must be source from the same program, assume first element
    if (tlevision_input.a_tle[0].source == TLE_SOURCE_PROGRAM) {
        status = application_output_read_json(&a_tle_output, execution_settings.working_directory, tlevision_input.a_tle[0].source_program_id, tlevision_input.a_tle[0].source_program_name);
        if (status != OK) {
            LOG(ERROR, "Failed to read program output: %s %s", tlevision_input.a_tle[0].source_program_id, tlevision_input.a_tle[0].source_program_name);
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
    LOG(INFO, "Loading TSPN into object");
    status = application_inputs_tspn_load(&tspn, &tlevision_input.tspn, &tspn_output, &execution_settings, program_name);
    if (status != OK) {
        LOG(ERROR, "Failed to load timespan");
        return ERROR;
    }
    LOG(INFO, "Loaded TSPN object successfully");

    // Load the TLEs
    LOG(INFO, "Loading TLEs into object(s)");
    status = tle_load(&a_tle, &n_tles, tlevision_input.a_tle, tlevision_input.n_tle, &a_tle_output, &execution_settings, tle_cat);
    if (status != OK) {
        LOG(ERROR, "Failed to load TLEs");
        return ERROR;
    }
    LOG(INFO, "Loaded TLEs successfully", n_tles);

    // Generate the ephemeris with SGP4 model
    LOG(INFO, "Generating ephemerides with SGP4");
    status = ephm_generate_SGP4(ephm_output, &n_ephm_output, &tspn, a_tle, n_tles, tlevision_input.wgs_model);
    if (status != OK) {
        LOG(ERROR, "Failed to generate ephemerides");
        return ERROR;
    }
    LOG(INFO, "Generated ephemerides successfully");


// StatusCode ephm_generate_SGP4(
//         // Outputs
//         ParameterEvolutionFile *a_ephm,
//         int *n_ephm,
//         // Inputs
//         const ParameterEvolution *timespan,
//         const tle_t *a_tle,
//         const int n_tles,
//         const int wgs_model) {
    LOG(INFO, "Program complete: %s (%s)", program_name, execution_settings.run_title);

    // Free memory
    if (tlevision_input.a_tle[0].source == TLE_SOURCE_CATALOG) cJSON_Delete(tle_cat);
    if (tlevision_input.tspn.source == TSPN_SOURCE_PROGRAM) application_output_free(&tspn_output);
    if (tlevision_input.a_tle[0].source == TLE_SOURCE_PROGRAM) application_output_free(&a_tle_output);
    free (a_tle);
    close_log();

    return OK;
}

