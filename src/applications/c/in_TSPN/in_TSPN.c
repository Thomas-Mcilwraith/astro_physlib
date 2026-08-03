/*
 * in_TLE.c
 *
 * Author: Thomas McIlwraith
 * Date: 01/08/2026
 *
 * This program writes user-defined Timespans to TSPN files in the workspace
 * 
 */

#include "in_TSPN_inputs.h"
#include "file-io/internal-products/parameter-evolution-file/parameter_evolution_file.h"
#include "mathematics-library/numerical-methods/interpolation/interpolation.h"
#include "utilities/constants/constants.h"
#include "reference-systems/time-systems/time-formats/time_formats.h"

int main(int argc, char *argv[]) {

    // Program variables
    StatusCode status = OK;
    execution_settings_t execution_settings;
    application_output_t output_file = {0};
    in_TSPN_inputs_t in_TSPN = {0};

    datetime_t start_date = {0}, end_date = {0};
    double start_jd = 0.0, end_jd = 0.0, step_days = 0.0;
    bool start_is_utc = false, end_is_utc = false;
    double *time_array = NULL;
    int n_points = 0;
    parameter_evolution_t timespan = {.name = UTC, .units = JD, .values = NULL, .n_values = 0};
    parameter_evolution_file_t file = { .parameters = NULL, .n_parameters = 0, .filename = NULL, .type = NULL, .source = NULL, .reference = NULL, .comment = NULL };
    char filename[256];

    // Parse command line, initialise log file
    status = parse_cmdline(&execution_settings, argc, argv);
    if (status != OK) {
        LOG(ERROR, "Failed to parse command line");
        return ERROR;
    }
    init_log(execution_settings.run_title, execution_settings.working_directory, execution_settings.program_name);

    // Load the inputs
    status = in_TSPN_inputs_read(&in_TSPN, execution_settings.working_directory, execution_settings.run_title);
    if (status != OK) {
        LOG(ERROR, "Failed to load inputs");
        return ERROR;
    }

    // Get the date objects
    status = iso8601_to_date(&start_date, &start_is_utc, in_TSPN.start_time_iso);
    status |= iso8601_to_date(&end_date, &end_is_utc, in_TSPN.end_time_iso);
    if (status != OK) {
        LOG(ERROR, "Failed to convert start and end times to dates");
        return ERROR;
    }

    // Get the JD objects
    status = date_to_jd(&start_jd, start_date);
    status |= date_to_jd(&end_jd, end_date);
    if (status != OK) {
        LOG(ERROR, "Failed to convert start and end dates to JD");
        return ERROR;
    }

    // Get the step size
    step_days = in_TSPN.stepsize_seconds / SECONDS_PER_DAY;

    // Generate the array
    status = generate_linearly_spaced_array(&(timespan.values), &(timespan.n_values), start_jd, end_jd, step_days);
    if (status != OK) {
        LOG(ERROR, "Failed to generate linearly spaced array");
        return ERROR;
    }

    LOG(INFO, "Generated timespan array with %d data points", timespan.n_values);

    // Write the parameter evolution file
    file.parameters = &timespan;
    file.n_parameters = 1;
    file.type = strdup(PEVF_TYPE_TSPN);
    file.source = strdup(PEVF_USER_DEFINED);
    file.reference = strdup(NO_REFERENCE);
    file.comment = NULL;
    snprintf(filename, sizeof(filename), "%s_TSPN.pev", execution_settings.run_title);
    file.filename = strdup(filename);

    status = parameter_evolution_file_write(file, execution_settings.working_directory);
    if (status != OK) {
        LOG(ERROR, "Failed to write parameter evolution file");
        return ERROR;
    }
    LOG(INFO, "TSPN file written: %s", filename);

    status = application_output_add_TSPN(&output_file, filename);
    if (status != OK) {
        LOG(ERROR, "Failed to add TSPN file to application output");
        return ERROR;
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
