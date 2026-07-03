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

    if (argc != 4) {
        LOG(ERROR, "Incorrect number of CMD line arguments (%d)", argc);
        return ERROR;
    }

    threads = atoi(argv[3]);
    if (threads < 1) {
        LOG(ERROR, "Number of threads must be greater than 0");
        return ERROR;
    }

    cfg->run_title = argv[1];
    cfg->working_directory = argv[2];
    cfg->n_threads = threads;

    return OK;
}

