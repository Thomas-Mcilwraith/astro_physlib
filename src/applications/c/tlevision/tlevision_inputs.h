#ifndef TLEVISION_INPUTS_H
#define TLEVISION_INPUTS_H

// Standard libraries

// Local libraries
#include "utilities/logging/log/log.h"
#include "utilities/misc/paths/paths.h"
#include "external/cjson/cJSON.h"
#include "file-io/internal-products/read-json/read_json.h"

// Variable and Macro definitions

// Function prototypes

typedef struct {
    char *wgs_model;
} tlevision_inputs_t;

StatusCode tlevision_inputs_read(
    // Outputs
    tlevision_inputs_t *inputs,
    // Inputs
    const char *working_directory,
    const char *run_title
    );

#endif
