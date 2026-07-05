#ifndef PARSE_CMDLINE_H
#define PARSE_CMDLINE_H

// Standard libraries
#include <stdlib.h>

// Local libraries
#include "utilities/logging/log/log.h"

// Variable and Macro definitions

#define WORKDIR_INPUTS "inputs"
#define WORKDIR_OUTPUTS "outputs"
#define WORKDIR_FILES "files"

// Function prototypes

/**
 * @brief All execution settings which are read from the command line
 *
 * @param run_title Run title for this execution
 * @param working_directory Working directory
 * @param n_threads Number of threads
 */
typedef struct {
    char *run_title;
    char *working_directory;
    int n_threads;
} ExecutionSettings;

/**
 * @brief Parses the command line arguments
 *
 * @param cfg Execution settings
 * @param argc Number of command line arguments
 * @param argv Command line arguments
 * @return Status code
 */
StatusCode parse_cmdline(
        // Outputs
        ExecutionSettings *cfg,
        // Inputs
        const int argc,
        char *argv[]);

#endif
