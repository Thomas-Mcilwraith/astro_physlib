#ifndef APPLICATION_OUTPUT_H
#define APPLICATION_OUTPUT_H

// Standard libraries

// Local libraries
#include "external/cjson/cJSON.h"
#include "file-io/internal-products/read-json/read_json.h"
#include "utilities/logging/log/log.h"
#include "utilities/misc/paths/paths.h"

// Variable and Macro definitions
#define MAX_OUTPUT_FILES 100

// Function prototypes

typedef struct {
    char *run_title;
    int n_TLE;
    char *aTLE[MAX_OUTPUT_FILES];
    int n_EPHM;
    char *aEPHM[MAX_OUTPUT_FILES];
    int n_TSPN;
    char *aTSPN[MAX_OUTPUT_FILES];
} application_output_t;

/**
 * @brief Read an application output file
 *
 * @param application_output Pointer to the application_output_t struct
 * @param working_directory Path to the working directory
 * @param run_title Title of the program to read outputs from
 * @return StatusCode OK if successful, ERROR otherwise
 */
StatusCode application_output_read(
    // Outputs
    application_output_t *application_output,
    // Inputs
    const char* working_directory,
    const char* run_title);

/**
 * @brief Free the application_output_t struct
 *
 * @param application_output Pointer to the application_output_t struct
 * @return StatusCode OK if successful, ERROR otherwise
 */
StatusCode application_output_free(application_output_t *application_output);


#endif
