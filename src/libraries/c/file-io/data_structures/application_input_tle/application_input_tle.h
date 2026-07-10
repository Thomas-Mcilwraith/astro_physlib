#ifndef APPLICATION_INPUT_TLE_H
#define APPLICATION_INPUT_TLE_H

// Standard libraries
#include <stdlib.h>

// Local libraries
#include "external/cjson/cJSON.h"
#include "utilities/logging/log/log.h"

// Variable and Macro definitions

#define TLE_SOURCE_USER 0
#define TLE_SOURCE_CATALOG 1
#define TLE_SOURCE_PROGRAM 2

// Function prototypes

typedef struct {
    int source;
    // if source == 0
    char* object_name;
    char* tle_line_1;
    char* tle_line_2;
    // if source == 1
    char* object_cospar_id;
    // if source == 2
    char* source_program_id;
    char* source_program_name;
} application_input_tle_t;

StatusCode application_input_tle_read_json(
    // Outputs
    application_input_tle_t *tle_input,
    // Inputs
    const cJSON *tle_json);

void application_input_tle_free(application_input_tle_t *tle_input);

#endif
