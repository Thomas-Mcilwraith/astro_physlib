#ifndef TLEVISION_INTERFACES_H
#define TLEVISION_INTERFACES_H

// Standard libraries

// Local libraries
#include "utilities/logging/log/log.h"
#include "utilities/misc/parse-cmdline/parse_cmdline.h"
#include "external/cjson/cJSON.h"
#include "file-io/internal-products/read-json/read_json.h"

// Variable and Macro definitions

// Function prototypes

typedef struct {

    int timespan_source;  // 0 = manual timespan input, 1 = from PEV file, 2 = from other program outputs
    char *iso8601_start_time;  // required if timespan_source == 0
    char *iso8601_stop_time;
    double step_size_seconds;
    char *pev_filename;  // required if timespan_source == 1
    char *timespan_source_id; // required if timespan_source == 2
    char *timespan_source_program;

    int tle_data_source;  // 0 = manual tle input, 1 = search database for id(s), 2 = from other program outputs
    char *tle_line_1;   // required if tle_data_source == 0
    char *tle_line_2; 
    // TODO: Add support for this in JSON
    char *object_name;  // optional if tle_data_source == 0
    char **object_ids;  // required if tle_data_source == 1
    int n_object_ids;
    char *tle_source_id;  // required if tle_data_source == 2
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
