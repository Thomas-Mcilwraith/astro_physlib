/*
 * test_application_main_options.c
 *
 * Author: Thomas McIlwraith
 * Date: 03/07/2026
 * 
 */

#include "test_application_inputs.h"

StatusCode read_TestApplicationInputs(
        // Outputs
        TestApplicationInputs *options,
        // Inputs
        const char *filename) {

    // Local variables
    cJSON *json;
    StatusCode status = OK;

    // Read the JSON file
    status = read_json(&json, filename);
    if (status != OK) {
        LOG(ERROR, "Failed to read JSON file: %s", filename);
        return ERROR;
    }

    // Assign all the static values
    options->year       = cJSON_GetObjectItemCaseSensitive(json, "year")->valueint;
    options->month      = cJSON_GetObjectItemCaseSensitive(json, "month")->valueint;
    options->day        = cJSON_GetObjectItemCaseSensitive(json, "day")->valueint;
    options->hour       = cJSON_GetObjectItemCaseSensitive(json, "hour")->valueint;
    options->minute     = cJSON_GetObjectItemCaseSensitive(json, "minute")->valueint;
    options->seconds    = cJSON_GetObjectItemCaseSensitive(json, "seconds")->valuedouble;
    options->utc_ut1_sec= cJSON_GetObjectItemCaseSensitive(json, "utc_ut1_sec")->valuedouble;
    options->xp         = cJSON_GetObjectItemCaseSensitive(json, "xp")->valuedouble;
    options->yp         = cJSON_GetObjectItemCaseSensitive(json, "yp")->valuedouble;
    options->dx_CIP     = cJSON_GetObjectItemCaseSensitive(json, "dx_CIP")->valuedouble;
    options->dy_CIP     = cJSON_GetObjectItemCaseSensitive(json, "dy_CIP")->valuedouble;

    // Assign the static arrays
    cJSON *vec = cJSON_GetObjectItemCaseSensitive(json, "gcrf_vec");
    for (int i = 0; i < 3; i++) {
        options->gcrf_vec[i] = cJSON_GetArrayItem(vec, i)->valuedouble;
    }

    // Retrieve the length of all variables arrays
    cJSON *extra = cJSON_GetObjectItemCaseSensitive(json, "extra_values");
    options->n_extra_values = cJSON_GetArraySize(extra);
    options->extra_values = malloc(options->n_extra_values * sizeof(double));
    for (int i = 0; i < options->n_extra_values; i++) {
        options->extra_values[i] =
            cJSON_GetArrayItem(extra, i)->valuedouble;
    }

    cJSON_Delete(json);
    return OK;
}

void free_TestApplicationInputs(TestApplicationInputs *options) {
    free(options->extra_values);
}
