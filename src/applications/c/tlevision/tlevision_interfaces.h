#ifndef TLEVISION_INTERFACES_H
#define TLEVISION_INTERFACES_H

// Standard libraries

// Local libraries
#include "utilities/logging/log/log.h"
#include "utilities/misc/parse-cmdline/parse_cmdline.h"
#include "external/cjson/cJSON.h"
#include "file-io/internal-products/read-json/read_json.h"

// Variable and Macro definitions
#define TLEVISION_INPUTS_TSPN_MANUAL 0
#define TLEVISION_INPUTS_TSPN_PEV 1
#define TLEVISION_INPUTS_TSPN_PROGRAM 2
#define TLEVISION_INPUTS_TLE_MANUAL 
#define TLEVISION_INPUTS_TLE_ID 1
#define TLEVISION_INPUTS_TLE_PROGRAM 2

// Function prototypes

typedef struct {

    int timespan_source;  // 0 = manual timespan input, 1 = from PEV file, 2 = from other program outputs
    int tle_data_source;  // 0 = manual tle input, 1 = search database for id(s), 2 = from other program outputs

    // Required if timespan_source = 0
    char *iso8601_start_time;
    char *iso8601_stop_time;
    double step_size_seconds;
    // Required if timespan_source = 1
    char *pev_filename;
    // Required if timespan_source = 2
    char *timespan_source_id;
    char *timespan_source_program;

    // Required if tle_data_source = 0
    char *tle_line_1;
    char *tle_line_2; 
    char *object_name;
    // Required if tle_data_source = 1
    char **object_ids;
    int n_object_ids;
    // Required if tle_data_source = 2
    char *tle_source_id;
    char *tle_source_program;
    
} tlevision_inputs_t;

typedef struct {
    
} tlevision_outputs_t;

StatusCode read_tlevision_inputs(
    // Outputs
    tlevision_inputs_t *inputs,
    // Inputs
    const char *working_directory,
    const char *run_title,
    const char *program_name);

void tlevision_inputs_free(tlevision_inputs_t *options);

#endif
