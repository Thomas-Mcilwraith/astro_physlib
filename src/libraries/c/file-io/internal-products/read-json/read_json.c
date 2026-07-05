/*
 * read_json.c
 *
 * Author: Thomas McIlwraith
 * Date: 18/04/2026
 */

#include "read_json.h"

StatusCode read_json(
        // Outputs
        cJSON **json,
        // Inputs
        const char *filename) {

    // Read the JSON file
    char *json_string = read_file(filename);
    if (!json_string) {
        LOG(ERROR, "Error reading file");
        return ERROR;
    }

    // Parse the JSON string
    *json = cJSON_Parse(json_string);
    if (!json) {
        LOG(ERROR, "Failed to parse JSON");
        return ERROR;
    }

    // Free the JSON string
    free(json_string);

    return OK;
}

char *read_file(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL)
        return NULL;

    if (fseek(fp, 0, SEEK_END) != 0) {
        fclose(fp);
        return NULL;
    }

    long size = ftell(fp);
    if (size < 0) {
        fclose(fp);
        return NULL;
    }

    rewind(fp);

    char *buffer = malloc((size_t)size + 1);
    if (buffer == NULL) {
        fclose(fp);
        return NULL;
    }

    size_t bytes_read = fread(buffer, 1, (size_t)size, fp);
    fclose(fp);

    if (bytes_read != (size_t)size) {
        free(buffer);
        return NULL;
    }

    buffer[size] = '\0';
    return buffer;
}
