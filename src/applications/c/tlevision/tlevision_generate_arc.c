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
#include "utilities/misc/parse-cmdline/parse_cmdline.h"

StatusCode ephm_generate_SGP4(
        // Outputs
        ParameterEvolutionFile **a_ephm,
        int *n_ephm,
        // Inputs
        const ParameterEvolution *timespan,
        const tle_t *a_tle,
        const int n_tles,
        const int wgs_model,
        const char* run_title) {

    // Local variables
    StatusCode status = OK;
    TLE satrec = {0};
    double r_km_buf[3], v_kms_buf[3];
    double minutes_after_epoch;
    ParameterEvolution r0, r1, r2, v0, v1, v2;
    char buf[256];

    *n_ephm = n_tles;

    if (n_tles <= 0) {
        LOG(WARNING, "No TLEs to generate arcs from");
        return WARNING;
    }

    // Allocate the pevfs
    *a_ephm = calloc(n_tles, sizeof(ParameterEvolutionFile));
    if (a_ephm == NULL) {
        LOG(ERROR, "Failed to allocate memory for ephm");
        return ERROR;;
    }

    // Loop over all the TLEs
    for (int i = 0; i < n_tles; i++) {
        // Assign the metadata for this ephemeris pevf
        (*a_ephm)[i].type = strdup(PEVF_TYPE_EPHM_NO_COV);
        if ((*a_ephm)[i].type == NULL) {
            LOG(ERROR, "Failed to allocate memory for ephm type");
            return ERROR;;
        }

        (*a_ephm)[i].reference = strdup(PEVF_REFERENCE_TEME);
        if ((*a_ephm)[i].reference == NULL) {
            LOG(ERROR, "Failed to allocate memory for ephm reference");
            return ERROR;;
        }

        (*a_ephm)[i].source = strdup(PEVF_TLEVISION_SGP4);
        if ((*a_ephm)[i].source == NULL) {
            LOG(ERROR, "Failed to allocate memory for ephm source");
            return ERROR;;
        }

        snprintf(buf, sizeof(buf), "Generated from TLE for object: %s", a_tle[i].object_id);
        (*a_ephm)[i].comment = strdup(buf);
        if ((*a_ephm)[i].comment == NULL) {
            LOG(ERROR, "Failed to allocate memory for ephm comment");
            return ERROR;;
        }

        snprintf(buf, sizeof(buf), "%s_EPHM_TLEVISION_%s.pev", run_title, a_tle[i].object_id);
        (*a_ephm)[i].filename = strdup(buf);
        if ((*a_ephm)[i].filename == NULL) {
            LOG(ERROR, "Failed to allocate memory for ephm filename");
            return ERROR;;
        }
       
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

        // Allocate space for the data arrays
        r0.values = malloc(timespan->n_values * sizeof(double));
        r1.values = malloc(timespan->n_values * sizeof(double));
        r2.values = malloc(timespan->n_values * sizeof(double));
        v0.values = malloc(timespan->n_values * sizeof(double));
        v1.values = malloc(timespan->n_values * sizeof(double));
        v2.values = malloc(timespan->n_values * sizeof(double));
        if (!r0.values || !r1.values || !r2.values || !v0.values || !v1.values || !v2.values) {
            LOG(ERROR, "Failed to allocate memory for ephemeris data arrays for object: %s", a_tle[i].object_id);
            return ERROR;
        }

        // Set the metatdata
        r0.units = r1.units = r2.units = KM;
        v0.units = v1.units = v2.units = KMS;
        r0.name = POS_X; r1.name = POS_Y; r2.name = POS_Z;
        v0.name = VEL_X; v1.name = VEL_Y; v2.name = VEL_Z;
        r0.n_values = r1.n_values = r2.n_values = v0.n_values = v1.n_values = v2.n_values = timespan->n_values;

        LOG(INFO, "Generating ephemeris for TLE: %s", a_tle[i].object_id);

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

            // Fill the data arrays
            r0.values[j] = r_km_buf[0];
            r1.values[j] = r_km_buf[1];
            r2.values[j] = r_km_buf[2];
            v0.values[j] = v_kms_buf[0];
            v1.values[j] = v_kms_buf[1];
            v2.values[j] = v_kms_buf[2];
        }

        // Add the data to the pevf
        (*a_ephm)[i].parameters = malloc(7 * sizeof(ParameterEvolution));
        if ((*a_ephm)[i].parameters == NULL) {
            LOG(ERROR, "Failed to allocate memory for ephemeris parameters for object: %s", a_tle[i].object_id);
            return ERROR;
        }
        (*a_ephm)[i].parameters[0] = *timespan;
        (*a_ephm)[i].parameters[1] = r0;
        (*a_ephm)[i].parameters[2] = r1;
        (*a_ephm)[i].parameters[3] = r2;
        (*a_ephm)[i].parameters[4] = v0;
        (*a_ephm)[i].parameters[5] = v1;
        (*a_ephm)[i].parameters[6] = v2;
        (*a_ephm)[i].n_parameters = 7;
    }

    return OK;
}
