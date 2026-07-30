/*
 * parse_cmdline.c
 *
 * Author: Thomas McIlwraith
 * Date: 19/06/2026
 */

#include "parse_cmdline.h"

StatusCode parse_cmdline(
        // Outputs
        ExecutionSettings *cfg,
        // Inputs
        const int argc,
        char *argv[]) {

    // Local variables
    int threads;

    LOG(INFO, "Parsing CMD line");

    if (argc != 4) {
        LOG(ERROR, "Incorrect number of CMD line arguments (%d)", argc);
        LOG(ERROR, "Usage: %s <run_title> <working_directory> <threads>", argv[0]);
        return ERROR;
    }

    threads = atoi(argv[3]);
    if (threads < 1) {
        LOG(ERROR, "Number of threads must be greater than 0");
        return ERROR;
    }

    cfg->program_name = strdup(get_program_name(argv));
    if (cfg->program_name == NULL) {
        LOG(ERROR, "Failed to allocate memory for program name");
        return ERROR;
    }

    cfg->run_title = argv[1];
    cfg->working_directory = argv[2];
    cfg->n_threads = threads;

    return OK;
}

const char *get_program_name(char *argv[]) {
    const char *name = strrchr(argv[0], '/');
    return (name != NULL) ? name + 1 : argv[0];
}

