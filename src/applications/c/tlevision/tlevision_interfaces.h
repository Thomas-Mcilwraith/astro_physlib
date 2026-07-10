#ifndef TLEVISION_INTERFACES_H
#define TLEVISION_INTERFACES_H

// Standard libraries

// Local libraries
#include "file-io/data_structures/application_input_tle/application_input_tle.h"
#include "file-io/data_structures/application_input_tspn/application_input_tspn.h"
#include "file-io/data_structures/application_output/application_output.h"
#include "utilities/logging/log/log.h"
#include "utilities/misc/parse-cmdline/parse_cmdline.h"
#include "utilities/misc/paths/paths.h"
#include "external/cjson/cJSON.h"
#include "file-io/internal-products/read-json/read_json.h"

// Variable and Macro definitions

// Function prototypes

typedef struct {
    int wgs_model;
    application_input_tspn_t tspn;
    application_input_tle_t *a_tle;
    int n_tle;
} tlevision_input_t;

StatusCode read_tlevision_inputs(
    // Outputs
    tlevision_input_t *inputs,
    // Inputs
    const char *working_directory,
    const char *run_title,
    const char *program_name);

#endif
