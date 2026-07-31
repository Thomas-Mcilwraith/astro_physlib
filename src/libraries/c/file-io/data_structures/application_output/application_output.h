#ifndef APPLICATION_OUTPUT_H
#define APPLICATION_OUTPUT_H

// Standard libraries

// Local libraries
#include "external/cjson/cJSON.h"
#include "file-io/data_structures/tle/tle.h"
#include "file-io/internal-products/read-json/read_json.h"
#include "utilities/logging/log/log.h"
#include "utilities/misc/paths/paths.h"

// Variable and Macro definitions

// Function prototypes

typedef struct {

    int n_TLE;
    char **aTLE;
    int n_EPHM;
    char **aEPHM;
    int n_TSPN;
    char **aTSPN;

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

StatusCode application_output_write(
    // Inputs
    const application_output_t *application_output,
    // Inputs
    const char* working_directory,
    const char* run_title);

StatusCode application_output_read_aTLE(
    // Outputs
    tle_t **aTLE,
    int *n_TLE,
    // Inputs
    const application_output_t *application_output,
    const char* working_directory
    );

StatusCode application_output_add_TLE(
    // Outputs
    application_output_t *application_output,
    // Inputs
    const char *filename);

StatusCode application_output_add_EPHM(
    // Outputs
    application_output_t *application_output,
    // Inputs
    const char *filename);

StatusCode application_output_add_TSPN(
    // Outputs
    application_output_t *application_output,
    // Inputs
    const char *filename);

// TODO: Fill these out, same as application_output_read_aTLE
StatusCode application_output_read_aEPHM();
StatusCode application_output_read_aTSPN();

/**
 * @brief Free the application_output_t struct
 *
 * @param application_output Pointer to the application_output_t struct
 * @return StatusCode OK if successful, ERROR otherwise
 */
StatusCode application_output_free(application_output_t *application_output);


#endif
