#ifndef PARSE_CMDLINE_H
#define PARSE_CMDLINE_H

// Standard libraries
#include <stdlib.h>

// Local libraries
#include "utilities/logging/log/log.h"
#include "utilities/misc/paths/paths.h"

// Variable and Macro definitions

// Function prototypes

/**
 * @brief All execution settings which are read from the command line
 *
 * @param run_title Run title for this execution
 * @param working_directory Working directory
 * @param n_threads Number of threads
 */
typedef struct {
    char *program_name;
    char *run_title;
    char *working_directory;
    int n_threads;
} execution_settings_t;

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
        execution_settings_t *cfg,
        // Inputs
        const int argc,
        char *argv[]);

/**
 * @brief Gets the program name from the command line arguments
 *
 * @param argv Command line arguments
 * @return Program name
 */
const char *get_program_name(char *argv[]);

#endif
