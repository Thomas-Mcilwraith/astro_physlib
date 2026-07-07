#ifndef TLEVISION_INTERFACES_H
#define TLEVISION_INTERFACES_H

// Standard libraries

// Local libraries
#include "utilities/logging/log/log.h"

// Variable and Macro definitions

// Function prototypes

typedef struct {

    int timespan_source;  // 0 = manual timespan input, 1 = from PEV file
    char *iso8601_start_time;  // required if timespan_source == 0
    char *iso8601_stop_time;
    double step_size_seconds;
    char *pev_filename;  // required if timespan_source == 1

    int tle_data_source;  // 0 = manual tle input, 1 = search database for id(s)
    char *tle_line_1;   // required if tle_data_source == 0
    char *tle_line_2; 
    char **object_ids;  // required if tle_data_source == 1
    int n_object_ids;
    
} tlevision_inputs_t;

typedef struct {
    
} tlevision_outputs_t;



#endif
