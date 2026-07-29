#ifndef PATHS_H
#define PATHS_H

// Standard libraries
#include <stdio.h>

// Local libraries
#include "utilities/logging/log/log.h"

// Variable and Macro definitions

#define FULL_PATH_BUFFER_SIZE 1024
#define FILE_NAME_BUFFER_SIZE 256

// Function prototypes

typedef enum {
    INPUTS,
    OUTPUTS,
    FILES,
    LOGS
} working_area_dir_t;

/**
 * Returns the full path to an input file
 *
 * @param full_path The full path to the input file
 * @param working_area The working area
 * @param working_area_dir The working area directory
 * @param filename The filename
 * @param buffer_size The size of the output buffer
 */
StatusCode working_area_path(
        // Outputs
        char* full_path,
        // Inputs
        const char* working_area,
        const working_area_dir_t file_type,
        const char* filename,
        const size_t buffer_size);

#endif
