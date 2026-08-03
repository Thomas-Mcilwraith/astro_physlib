/*
 * tle.c
 *
 * Author: Thomas McIlwraith
 * Date: 18/04/2026
 */

#include "tle.h"
#include "file-io/internal-products/read-json/read_json.h"

StatusCode tle_read_single_json(
    // Outputs
    tle_t *tle,
    // Inputs
    const cJSON *omm_json) {
    
    // TLE_LINE1
    const cJSON *tle_line1 = cJSON_GetObjectItemCaseSensitive(omm_json, "TLE_LINE1");
    if (!cJSON_IsString(tle_line1)) {
        LOG(ERROR, "Failed to load TLE_LINE1");
        return ERROR;
    }
    tle->tle_line1 = strdup(tle_line1->valuestring);
    if (tle->tle_line1 == NULL) {
        LOG(ERROR, "Failed to allocate memory for TLE_LINE1");
        return ERROR;
    }
    
    // TLE_LINE2
    const cJSON *tle_line2 = cJSON_GetObjectItemCaseSensitive(omm_json, "TLE_LINE2");
    if (!cJSON_IsString(tle_line2)) {
        LOG(ERROR, "Failed to load TLE_LINE2");
        return ERROR;
    }
    tle->tle_line2 = strdup(tle_line2->valuestring);
    if (tle->tle_line2 == NULL) {
        LOG(ERROR, "Failed to allocate memory for TLE_LINE2");
        return ERROR;
    }

    // OBJECT_ID
    const cJSON *object_id = cJSON_GetObjectItemCaseSensitive(omm_json, "OBJECT_ID");
    if (!cJSON_IsString(object_id)) {
        LOG(ERROR, "Failed to load OBJECT_ID");
        return ERROR;
    }
    tle->object_id = strdup(object_id->valuestring);
    if (tle->object_id == NULL) {
        LOG(ERROR, "Failed to allocate memory for OBJECT_ID");
        return ERROR;
    }

    return OK;
}

StatusCode tle_write_single_json(
    // Outputs
    char** written_filename,
    // Inputs
    const tle_t *tle,
    const char* working_directory,
    const char* run_title,
    const int file_number) {

    // Local variables
    StatusCode status = OK;
    char filename[FILE_NAME_BUFFER_SIZE], filepath[FULL_PATH_BUFFER_SIZE];
    int len_filename = 0;
    cJSON *json = cJSON_CreateObject();
    cJSON *tle_line1 = NULL;
    cJSON *tle_line2 = NULL;
    cJSON *object_id = NULL;

    if (tle == NULL) {
        LOG(ERROR, "Bad pointer to tle_write_single_json");
        return ERROR;
    }

    len_filename = snprintf(filename, FILE_NAME_BUFFER_SIZE, "%s_%d_TLE_%s.json", run_title, file_number, tle->object_id);
    if (len_filename >= FILE_NAME_BUFFER_SIZE) {
        LOG(ERROR, "Filename too long: %s", filename);
        return ERROR;
    }
    status = working_area_path(filepath, working_directory, FILES, filename, FULL_PATH_BUFFER_SIZE);
    if (status != OK) {
        LOG(ERROR, "Error constructing path to application output file: %s", filename);
        return status;
    }

    // Add data to the JSON object
    object_id = cJSON_AddStringToObject(json, "OBJECT_ID", tle->object_id);
    if (object_id == NULL) {
        LOG(ERROR, "Failed to add object_id to JSON object");
        return ERROR;
    }

    tle_line1 = cJSON_AddStringToObject(json, "TLE_LINE1", tle->tle_line1);
    if (tle_line1 == NULL) {
        LOG(ERROR, "Failed to add TLE_LINE1 to JSON object");
        return ERROR;
    }

    tle_line2 = cJSON_AddStringToObject(json, "TLE_LINE2", tle->tle_line2);
    if (tle_line2 == NULL) {
        LOG(ERROR, "Failed to add TLE_LINE2 to JSON object");
        return ERROR;
    }

    status = write_json(json, filepath);
    if (status != OK) {
        LOG(ERROR, "Failed to write JSON file: %s", filepath);
        return ERROR;
    }

    *written_filename = strdup(filename);    
    if (written_filename == NULL) {
        LOG(ERROR, "Failed to allocate memory for written_filename");
        return ERROR;
    }

    cJSON_Delete(json);

    return OK;
}

char *cospar_short_to_long(const char *short_cospar)
{
    if (short_cospar == NULL)
        return NULL;

    int yy = (short_cospar[0] - '0') * 10 + (short_cospar[1] - '0');
    int year = (yy >= 57) ? (1900 + yy) : (2000 + yy);

    char *long_id = malloc(10);
    if (long_id == NULL)
        return NULL;

    sprintf(long_id, "%04d-%.3s%c",
            year,
            short_cospar + 2,
            short_cospar[5]);

    return long_id;
}
