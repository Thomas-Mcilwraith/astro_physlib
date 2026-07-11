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
#include "external/sgp4/TLE.h"
#include "file-io/internal-products/parameter-evolution-file/parameter_evolution_file.h"
#include "utilities/constants/constants.h"

StatusCode ephm_generate_SGP4(
        // Outputs
        ParameterEvolutionFile *a_ephm,
        int *n_ephm,
        // Inputs
        const ParameterEvolution *timespan,
        const tle_t *a_tle,
        const int n_tles,
        const int wgs_model) {

    // Local variables
    StatusCode status = OK;
    TLE satrec = {0};
    double r_km_buf[3], v_kms_buf[3];
    double minutes_after_epoch;

    if (n_tles <= 0) {
        LOG(WARNING, "No TLEs to generate arcs from");
        return WARNING;
    }

    a_ephm = malloc(n_tles * sizeof(ParameterEvolutionFile));
    if (a_ephm == NULL) {
        LOG(ERROR, "Failed to allocate memory for ephm");
        return ERROR;;
    }

    // Loop over all the TLEs
    for (int i = 0; i < n_tles; i++) {

        LOG(INFO, "Generating ephemeris for TLE: %s", a_tle[i].object_id);
       
        // Load the TLE lines into the satrec
        parseLines(&satrec, a_tle[i].tle_line1, a_tle[i].tle_line2, wgs_model);
        if (satrec.sgp4Error != 0) {
            LOG(WARNING, "SGP4 error detected (%d) when parsing TLE for object %s", satrec.sgp4Error, a_tle[i].object_id);
            if (satrec.sgp4Error == 1) LOG(WARNING, "Mean elements, ecc >= 1.0 or ecc < -0.001 or a < 0.95 er");
            if (satrec.sgp4Error == 2) LOG(WARNING, "Mean motion less than 0.0");
            if (satrec.sgp4Error == 3) LOG(WARNING, "Pert elements, ecc < 0.0  or  ecc > 1.0");
            if (satrec.sgp4Error == 4) LOG(WARNING, "Semi-latus rectum < 0.0");
            if (satrec.sgp4Error == 5) LOG(WARNING, "Epoch elements are sub-orbital");
            if (satrec.sgp4Error == 6) LOG(WARNING, "Satellite has decayed");
            continue;
        }

        // Loop over the timespan
        for (int j = 0; j < timespan->n_values; j++) {

            // Compute the minutes after epoch
            minutes_after_epoch = (timespan->values[j] - (double) satrec.epoch) * MINUTES_PER_DAY;
            
            // Get the position and velocity at the timespan value
            getRV(&satrec, minutes_after_epoch, r_km_buf, v_kms_buf);
            if (satrec.rec.error != 0) {
                LOG(WARNING, "SGP4 error detected (%d) when propagating TLE for object %s", satrec.sgp4Error, a_tle[i].object_id);
                if (satrec.rec.error == 1) LOG(WARNING, "Mean elements, ecc >= 1.0 or ecc < -0.001 or a < 0.95 er");
                if (satrec.rec.error == 2) LOG(WARNING, "Mean motion less than 0.0");
                if (satrec.rec.error == 3) LOG(WARNING, "Pert elements, ecc < 0.0  or  ecc > 1.0");
                if (satrec.rec.error == 4) LOG(WARNING, "Semi-latus rectum < 0.0");
                if (satrec.rec.error == 5) LOG(WARNING, "Epoch elements are sub-orbital");
                if (satrec.rec.error == 6) LOG(WARNING, "Satellite has decayed");
                break;
            }

            LOG(INFO, "posvel 1: %f %f %f", r_km_buf[0], r_km_buf[1], r_km_buf[2]);
            LOG(INFO, "posvel 2: %f %f %f", v_kms_buf[0], v_kms_buf[1], v_kms_buf[2]);
            break;

        }

    }

    return OK;
}
