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
#include "file-io/internal-products/parameter-evolution-file/parameter_evolution_file.h"

StatusCode ephm_generate_arcs(
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

    if (n_tles <= 0) {
        LOG(WARNING, "No TLEs to generate arcs from");
        return WARNING;
    }

    a_ephm = malloc(n_tles * sizeof(ParameterEvolutionFile));
    if (a_ephm == NULL) {
        LOG(ERROR, "Failed to allocate memory for ephm");
        return ERROR;;
    }

    return OK;
}
