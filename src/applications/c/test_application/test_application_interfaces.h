#ifndef TEST_APPLICATION_INTERFACES_H
#define TEST_APPLICATION_INTERFACES_H

// Standard libraries

// Local libraries
#include "utilities/logging/log/log.h"
#include "external/cjson/cJSON.h"
#include "file-io/internal-products/read-json/read_json.h"
#include "utilities/misc/parse-cmdline/parse_cmdline.h"
#include "utilities/misc/paths/paths.h"

// Variable and Macro definitions

// Function prototypes

/**
 * @brief Contais the main options for the test application.
 */
typedef struct {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    double seconds;
    double gcrf_vec[3];
    double utc_ut1_sec;  // From EOP data
    double xp;           // From EOP data
    double yp;           // From EOP data
    double dx_CIP;       // From EOP data
    double dy_CIP;       // From EOP data
    int n_extra_values;
    double *extra_values;
} TestApplicationInputs;

/**
 * @brief Reads the main options from a JSON file.
 * 
 * @param options The main options to be read.
 * @param working_directory The working directory.
 * @param filename The name of the JSON file to read.
 * @param program_name the name of the program
 * @return OK if successful, ERROR otherwise.
 */
StatusCode read_TestApplicationInputs(
        // Outputs
        TestApplicationInputs *options,
        // Inputs
        const char *working_directory,
        const char *filename,
        const char *program_name);

/**
 * @brief Frees the main options.
 * 
 * @param options The main options to be freed.
 */
void free_TestApplicationInputs(TestApplicationInputs *options);

#endif
