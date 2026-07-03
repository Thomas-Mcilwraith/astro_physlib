#ifndef READ_JSON_H
#define READ_JSON_H

// Standard libraries
#include <stdlib.h>

// Local libraries
#include "utilities/logging/log/log.h"
#include "external/cjson/cJSON.h"

// Variable and Macro definitions

// Function prototypes

/**
 * @brief Reads a JSON file into a cJSON object
 *
 * @param json The cJSON object to populate
 * @param filename The name of the file to read
 * @return OK if successful, ERROR otherwise
 */
StatusCode read_json(
        // Outputs
        cJSON **json,
        // Inputs
        const char *filename);

/**
 * @brief Reads a file into a string
 *
 * @note The string must be freed by the caller
 *
 * @param filename The name of the file to read
 * @return A pointer to the file contents
 */
char *read_file(const char *filename);

#endif
