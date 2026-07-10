/*
 * application_input_tle.c
 *
 * Author: Thomas McIlwraith
 * Date: 18/04/2026
 */

#include "application_input_tle.h"

StatusCode application_input_tle_read_json(
    // Outputs
    application_input_tle_t *tle_input,
    // Inputs
    const cJSON *tle_json) {

    // Local variables

    // Assign all the static values
    const cJSON *source = cJSON_GetObjectItemCaseSensitive(tle_json, "source");
    if (!cJSON_IsNumber(source)) {
        LOG(ERROR, "Incorrect type in TLE: source");
        return ERROR;
    }
    tle_input->source = source->valueint;

    // Case source == 0 - Read the user defined inputs
    if (tle_input->source == TLE_SOURCE_USER) {

        const cJSON *object_name = cJSON_GetObjectItemCaseSensitive(tle_json, "object_name");
        if (!cJSON_IsString(object_name)) {
            LOG(ERROR, "Incorrect type in TLE: object_name");
            return ERROR;
        }
        tle_input->object_name = strdup(object_name->valuestring);

        const cJSON *tle_line_1 = cJSON_GetObjectItemCaseSensitive(tle_json, "tle_line_1");
        if (!cJSON_IsString(tle_line_1)) {
            LOG(ERROR, "Incorrect type in TLE: tle_line1");
            return ERROR;
        }
        tle_input->tle_line_1 = strdup(tle_line_1->valuestring);

        const cJSON *tle_line_2 = cJSON_GetObjectItemCaseSensitive(tle_json, "tle_line_2");
        if (!cJSON_IsString(tle_line_2)) {
            LOG(ERROR, "Incorrect type in TLE: tle_line2");
            return ERROR;
        }
        tle_input->tle_line_2 = strdup(tle_line_2->valuestring);


    // Case source == 1 - Retrieve TLE from the catalog
    } else if (tle_input->source == TLE_SOURCE_CATALOG) {

        const cJSON *object_cospar_id = cJSON_GetObjectItemCaseSensitive(tle_json, "object_cospar_id");
        if (!cJSON_IsString(object_cospar_id)) {
            LOG(ERROR, "Incorrect type in TLE: object_cospar_id");
            return ERROR;
        }
        tle_input->object_cospar_id = strdup(object_cospar_id->valuestring);

    // Case source == 2 - Read the program outputs
    } else if (tle_input->source == TLE_SOURCE_PROGRAM) {

        const cJSON *source_program_id = cJSON_GetObjectItemCaseSensitive(tle_json, "source_program_id");
        if (!cJSON_IsString(source_program_id)) {
            LOG(ERROR, "Incorrect type in TLE: source_program_id");
            return ERROR;
        }
        tle_input->source_program_id = strdup(source_program_id->valuestring);

        const cJSON *source_program_name = cJSON_GetObjectItemCaseSensitive(tle_json, "source_program_name");
        if (!cJSON_IsString(source_program_name)) {
            LOG(ERROR, "Incorrect type in TLE: source_program_name");
            return ERROR;
        }
        tle_input->source_program_name = strdup(source_program_name->valuestring);

    } else {
        LOG(ERROR, "Incorrect value in TLE: source = %d", tle_input->source);
        return ERROR;
    }

    return OK;
}

void application_input_tle_free(application_input_tle_t *tle_input) {
    if (tle_input == NULL) {
        return;
    }

    free(tle_input->object_name);
    free(tle_input->tle_line_1);
    free(tle_input->tle_line_2);
    free(tle_input->object_cospar_id);
    free(tle_input->source_program_id);
    free(tle_input->source_program_name);

    return;
}
