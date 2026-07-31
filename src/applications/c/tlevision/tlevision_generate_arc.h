#ifndef TLEVISION_GENERATE_ARC_H
#define TLEVISION_GENERATE_ARC_H

// Standard libraries
#include <pthread.h>

// Local libraries
#include "utilities/logging/log/log.h"
#include "utilities/constants/constants.h"
#include "utilities/misc/parse-cmdline/parse_cmdline.h"
#include "utilities/misc/paths/paths.h"
#include "file-io/data_structures/tle/tle.h"
#include "file-io/internal-products/parameter-evolution-file/parameter_evolution_file.h"
#include "external/sgp4/TLE.h"

// Variable and Macro definitions

// Function prototypes
StatusCode ephm_generate_SGP4(
        // Outputs
        parameter_evolution_file_t **a_ephm,
        int *n_ephm,
        // Inputs
        const parameter_evolution_t *timespan,
        const tle_t *a_tle,
        const int n_tles,
        const char *wgs_model,
        const char* run_title);

#endif
