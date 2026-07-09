/*
 * tlevision_timespan.c
 *
 * Author: Thomas McIlwraith
 * Date: 09/07/2026
 * 
 * Handles the loading of the timespan for tlevision.
 * 
 */

#include "tlevision_load_timespan.h"

StatusCode load_timespan(
        // Outputs
        ParameterEvolution *timespan,
        // Inputs
        const ParameterEvolutionFile *pev_with_timespan,
        const tlevision_inputs_t *inputs,
        const ExecutionSettings *execution_settings) {

    // Local variables
    StatusCode status = OK;
    datetime_t dt_start_time, dt_stop_time;
    double jd_start_time, jd_stop_time, jd_step_size;
    bool is_utc, found_timey_param;
    double *jd_timespan = NULL;
    int len_jd_timespan = 0;

    // If user inputs are taken, read the strings and generate the timespan
    if (inputs->timespan_source == TLEVISION_INPUTS_TSPN_MANUAL) {

        // Read the timestamps
        status = iso8601_to_date(&dt_start_time, &is_utc, inputs->iso8601_start_time);
        status |= iso8601_to_date(&dt_stop_time, &is_utc, inputs->iso8601_stop_time);
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
        jd_step_size = inputs->step_size_seconds / SECONDS_PER_DAY;
        status = generate_linearly_spaced_array(&jd_timespan, &len_jd_timespan,
                                                jd_start_time, jd_stop_time, jd_step_size);
        if (status != OK) {
            LOG(ERROR, "Failed to generate timespan %f -> %f", jd_start_time, jd_stop_time);
            return ERROR;
        }

        // Construct the Parameter Evolution
        timespan->name = UTC;
        timespan->units = JD;
        timespan->n_values = len_jd_timespan;
        timespan->values = jd_timespan;

    // If the timespan is taken from a specific PEV file,
    // pev_with_timespan is already loaded
    } else if (inputs->timespan_source == TLEVISION_INPUTS_TSPN_PEV) {

        status = parameter_evolution_file_get_jd(timespan, pev_with_timespan);
        if (status != OK) {
            LOG(ERROR, "Failed to retrieve timespan from PEV file");
            return ERROR;
        }

    // If the timespan is taken from another programs outputs,
    // pev_with_timespan is already loaded
    } else if (inputs->timespan_source == TLEVISION_INPUTS_TSPN_PROGRAM) {

        status = parameter_evolution_file_get_jd(timespan, pev_with_timespan);
        if (status != OK) {
            LOG(ERROR, "Failed to retrieve timespan from PEV file");
            return ERROR;
        }

    }

    free(jd_timespan);
    return OK;
}
