/*
 * tlevision_generate_arc.c
 *
 * Author: Thomas McIlwraith
 * Date: 63/07/2026
 * 
 * Module for generating arcs from TLEs.
 * 
 */

#include "tlevision_generate_arc.h"

StatusCode SPG4_generate_arc(
        // Outputs
        // TODO: this should be parameter evolutionFile
        ParameterEvolution **arcs,
        int *n_arcs,
        // Inputs
        const ParameterEvolution *timespan,
        const tle_t *a_tle,
        const int n_tles,
        const int wgs_model) {

    // Local variables
    StatusCode status = OK;
    TLE *tle_temp = {0};
    double minutes_after_epoch;
    double r_km[3], v_kms[3];

    if (n_tles <= 0) {
        LOG(WARNING, "No TLEs to generate arcs from");
        return WARNING;
    }

    *arcs = malloc(n_tles * sizeof(ParameterEvolution));
    if (*arcs == NULL) {
        LOG(ERROR, "Failed to allocate memory for arcs");
        return ERROR;
    }

    for (int i = 0; i < n_tles; i++) {
        LOG(INFO, "Generating arc for TLE: %s", a_tle[i].object_id);
        parseLines(tle_temp, a_tle[i].tle_line1, a_tle[i].tle_line2, wgs_model);

        for (int j = 0; j < timespan->n_values; j++) {
            minutes_after_epoch = (timespan->values[j] - tle_temp->epoch) * MINUTES_PER_DAY;
            getRV(tle_temp, minutes_after_epoch, r_km, v_kms);


        }

        // Construct the parameter evolution
        arcs[i]->name = UTC;
        arcs[i]->units = KM;
    }
    
    *n_arcs = n_tles;


    return OK;
}
