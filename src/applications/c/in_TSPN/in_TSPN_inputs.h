#ifndef IN_TLE_INPUTS_H
#define IN_TLE_INPUTS_H

// Standard libraries

// Local libraries
#include "file-io/internal-products/read-json/read_json.h"
#include "utilities/logging/log/log.h"
#include "utilities/misc/parse-cmdline/parse_cmdline.h"
#include "file-io/data_structures/application_output/application_output.h"

// Variable and Macro definitions

// Function prototypes

typedef struct {
    char *start_time_iso;
    char *end_time_iso;
    double stepsize_seconds;
} in_TSPN_inputs_t;

StatusCode in_TSPN_inputs_read(
    // Outputs
    in_TSPN_inputs_t *in_TSPN,
    // Inputs
    const char *working_directory,
    const char *run_title
    );

#endif
