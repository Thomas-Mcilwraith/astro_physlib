/*
 * test_application_main_options.c
 *
 * Author: Thomas McIlwraith
 * Date: 03/07/2026
 * 
 */

#include "test_application_interfaces.h"

StatusCode read_TestApplicationInputs(
        // Outputs
        TestApplicationInputs *options,
        // Inputs
        const char *working_directory,
        const char *run_title) {

    // Local variables
    cJSON *json;
    StatusCode status = OK;
    char filepath[1024];

    // Construct the file path
    snprintf(filepath, sizeof(filepath), "%s/%s/%s.json", working_directory, WORKDIR_INPUTS, run_title);

    // Read the JSON file
    status = read_json(&json, filepath);
    if (status != OK) {
        LOG(ERROR, "Failed to read JSON file: %s", filepath);
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

    // Log all loaded values
    LOG(INFO, "Loaded Input: Year: %d", options->year);
    LOG(INFO, "Loaded Input: Month: %d", options->month);
    LOG(INFO, "Loaded Input: Day: %d", options->day);
    LOG(INFO, "Loaded Input: Hour: %d", options->hour);
    LOG(INFO, "Loaded Input: Minute: %d", options->minute);
    LOG(INFO, "Loaded Input: Seconds: %f", options->seconds);
    LOG(INFO, "Loaded Input: UTC_UT1_Sec: %f", options->utc_ut1_sec);
    LOG(INFO, "Loaded Input: XP: %f", options->xp);
    LOG(INFO, "Loaded Input: YP: %f", options->yp);
    LOG(INFO, "Loaded Input: DX_CIP: %f", options->dx_CIP);
    LOG(INFO, "Loaded Input: DY_CIP: %f", options->dy_CIP);
    LOG(INFO, "Loaded Input: GCRF Vec: %f    %f    %f", options->gcrf_vec[0], options->gcrf_vec[1], options->gcrf_vec[2]);
    LOG(INFO, "Loaded Input: Extra Values: %d", options->n_extra_values);
    for (int i = 0; i < options->n_extra_values; i++) {
        LOG(INFO, "Loaded Input: Extra Values[%d]: %f", i, options->extra_values[i]);
    }

    cJSON_Delete(json);
    return OK;
}

void free_TestApplicationInputs(TestApplicationInputs *options) {
    free(options->extra_values);
}
