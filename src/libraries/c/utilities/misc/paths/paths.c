/*
 * paths.c
 *
 * Author: Thomas McIlwraith
 * Date: 19/06/2026
 */

#include "paths.h"

StatusCode working_area_path(
        // Outputs
       char* full_path,
        // Inputs
        const char* working_area,
        const working_area_dir_t file_type,
        const char* filename,
        const size_t buffer_size) {

    // Local variables
    int snprintf_status;
    char* working_area_dir;

    switch (file_type) {
        case INPUTS:
            working_area_dir = "inputs";
            break;
        case OUTPUTS:
            working_area_dir = "outputs";
            break;
        case FILES:
            working_area_dir = "files";
            break;
        case LOGS:
            working_area_dir = "logs";
            break;
        default:
            LOG(ERROR, "Unknown working area directory");
            return ERROR;
    }

    snprintf_status = snprintf(full_path, buffer_size, "%s/%s/%s",
                               working_area,
                               working_area_dir,
                               filename);

    if (snprintf_status < 0) {
        LOG(ERROR, "Error constructing file path for %s/%s", working_area_dir, filename);
        return ERROR;
    } else if ((size_t)snprintf_status >= buffer_size) {
        LOG(ERROR, "Buffer size too small for file path for %s/%s", working_area_dir, filename);
        return ERROR;
    }

    return OK;
}

