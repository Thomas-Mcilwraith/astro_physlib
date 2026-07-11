#ifndef TLEVISION_GENERATE_ARC_H
#define TLEVISION_GENERATE_ARC_H

// Standard libraries
#include <pthread.h>

// Local libraries
#include "utilities/logging/log/log.h"
#include "utilities/constants/constants.h"
#include "file-io/data_structures/tle/tle.h"
#include "file-io/internal-products/parameter-evolution-file/parameter_evolution_file.h"

// Variable and Macro definitions

// Function prototypes
StatusCode ephm_generate_SGP4(
        // Outputs
        ParameterEvolutionFile *a_ephm,
        int *n_ephm,
        // Inputs
        const ParameterEvolution *timespan,
        const tle_t *a_tle,
        const int n_tles,
        const int wgs_model);

#endif
