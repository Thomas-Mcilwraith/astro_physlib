#ifndef APPLICATION_INPUT_TSPN_H
#define APPLICATION_INPUT_TSPN_H

// Standard libraries
#include <stdlib.h>

// Local libraries
#include "external/cjson/cJSON.h"
#include "utilities/logging/log/log.h"
#include "file-io/data_structures/application_output/application_output.h"
#include "file-io/internal-products/parameter-evolution-file/parameter_evolution_file.h"
#include "mathematics-library/numerical-methods/interpolation/interpolation.h"
#include "utilities/constants/constants.h"
#include "utilities/misc/parse-cmdline/parse_cmdline.h"

// Variable and Macro definitions
#define TSPN_SOURCE_USER 0
#define TSPN_SOURCE_PROGRAM 1

// Function prototypes

typedef struct {
    int source;
    // if source == 0
    char* iso8601_start_time;
    char* iso8601_stop_time;
    double step_size_seconds;
    // if source == 1
    char* source_program_id;
    char* source_program_name;

} application_input_tspn_t;

StatusCode application_input_tspn_read_json(
    // Outputs
    application_input_tspn_t *tspn_input,
    // Inputs
    const cJSON *tspn_json);

StatusCode application_inputs_tspn_load(
        // Outputs
        ParameterEvolution *tspn,
        // Inputs
        const application_input_tspn_t *tspn_inputs,
        const application_output_t *tspn_outputs,
        const ExecutionSettings *execution_settings,
        const char *program_name
        );

void application_input_tspn_free(application_input_tspn_t *tspn_input);

#endif
