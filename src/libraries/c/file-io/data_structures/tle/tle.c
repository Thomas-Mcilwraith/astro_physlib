/*
 * tle.c
 *
 * Author: Thomas McIlwraith
 * Date: 18/04/2026
 */

#include "tle.h"

StatusCode tle_read_single_json(
    // Outputs
    tle_t *tle,
    // Inputs
    const cJSON *omm_json) {
    
    // TLE_LINE0
    const cJSON *tle_line0 = cJSON_GetObjectItemCaseSensitive(omm_json, "TLE_LINE0");
    if (!cJSON_IsString(tle_line0)) {
        LOG(ERROR, "Failed to load TLE_LINE0");
        return ERROR;
    }
    tle->tle_line0 = strdup(tle_line0->valuestring);
    if (tle->tle_line0 == NULL) {
        LOG(ERROR, "Failed to allocate memory for TLE_LINE0");
        return ERROR;
    }
    
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

    // CCSDS_OMM_VERS
    const cJSON *ccsds_omm_vers = cJSON_GetObjectItemCaseSensitive(omm_json, "CCSDS_OMM_VERS");
    if (!cJSON_IsString(ccsds_omm_vers)) {
        LOG(ERROR, "Failed to load CCSDS_OMM_VERS");
        return ERROR;
    }
    tle->ccsds_omm_vers = strdup(ccsds_omm_vers->valuestring);
    if (tle->ccsds_omm_vers == NULL) {
        LOG(ERROR, "Failed to allocate memory for CCSDS_OMM_VERS");
        return ERROR;
    }

    // COMMENT
    const cJSON *comment = cJSON_GetObjectItemCaseSensitive(omm_json, "COMMENT");
    if (!cJSON_IsString(comment)) {
        LOG(ERROR, "Failed to load COMMENT");
        return ERROR;
    }
    tle->comment = strdup(comment->valuestring);
    if (tle->comment == NULL) {
        LOG(ERROR, "Failed to allocate memory for COMMENT");
        return ERROR;
    }

    // CREATION_DATE
    const cJSON *creation_date = cJSON_GetObjectItemCaseSensitive(omm_json, "CREATION_DATE");
    if (!cJSON_IsString(creation_date)) {
        LOG(ERROR, "Failed to load CREATION_DATE");
        return ERROR;
    }
    tle->creation_date = strdup(creation_date->valuestring);
    if (tle->creation_date == NULL) {
        LOG(ERROR, "Failed to allocate memory for CREATION_DATE");
        return ERROR;
    }

    // ORIGINATOR
    const cJSON *originator = cJSON_GetObjectItemCaseSensitive(omm_json, "ORIGINATOR");
    if (!cJSON_IsString(originator)) {
        LOG(ERROR, "Failed to load ORIGINATOR");
        return ERROR;
    }
    tle->originator = strdup(originator->valuestring);
    if (tle->originator == NULL) {
        LOG(ERROR, "Failed to allocate memory for ORIGINATOR");
        return ERROR;
    }

    // OBJECT_NAME
    const cJSON *object_name = cJSON_GetObjectItemCaseSensitive(omm_json, "OBJECT_NAME");
    if (!cJSON_IsString(object_name)) {
        LOG(ERROR, "Failed to load OBJECT_NAME");
        return ERROR;
    }
    tle->object_name = strdup(object_name->valuestring);
    if (tle->object_name == NULL) {
        LOG(ERROR, "Failed to allocate memory for OBJECT_NAME");
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

    // CENTER_NAME
    const cJSON *center_name = cJSON_GetObjectItemCaseSensitive(omm_json, "CENTER_NAME");
    if (!cJSON_IsString(center_name)) {
        LOG(ERROR, "Failed to load CENTER_NAME");
        return ERROR;
    }
    tle->center_name = strdup(center_name->valuestring);
    if (tle->center_name == NULL) {
        LOG(ERROR, "Failed to allocate memory for CENTER_NAME");
        return ERROR;
    }

    // REF_FRAME
    const cJSON *ref_frame = cJSON_GetObjectItemCaseSensitive(omm_json, "REF_FRAME");
    if (!cJSON_IsString(ref_frame)) {
        LOG(ERROR, "Failed to load REF_FRAME");
        return ERROR;
    }
    tle->ref_frame = strdup(ref_frame->valuestring);
    if (tle->ref_frame == NULL) {
        LOG(ERROR, "Failed to allocate memory for REF_FRAME");
        return ERROR;
    }

    // TIME_SYSTEM
    const cJSON *time_system = cJSON_GetObjectItemCaseSensitive(omm_json, "TIME_SYSTEM");
    if (!cJSON_IsString(time_system)) {
        LOG(ERROR, "Failed to load TIME_SYSTEM");
        return ERROR;
    }
    tle->time_system = strdup(time_system->valuestring);
    if (tle->time_system == NULL) {
        LOG(ERROR, "Failed to allocate memory for TIME_SYSTEM");
        return ERROR;
    }

    // MEAN_ELEMENT_THEORY
    const cJSON *mean_element_theory = cJSON_GetObjectItemCaseSensitive(omm_json, "MEAN_ELEMENT_THEORY");
    if (!cJSON_IsString(mean_element_theory)) {
        LOG(ERROR, "Failed to load MEAN_ELEMENT_THEORY");
        return ERROR;
    }
    tle->mean_element_theory = strdup(mean_element_theory->valuestring);
    if (tle->mean_element_theory == NULL) {
        LOG(ERROR, "Failed to allocate memory for MEAN_ELEMENT_THEORY");
        return ERROR;
    }

    // EPOCH
    const cJSON *epoch = cJSON_GetObjectItemCaseSensitive(omm_json, "EPOCH");
    if (!cJSON_IsString(epoch)) {
        LOG(ERROR, "Failed to load EPOCH");
        return ERROR;
    }
    tle->epoch = strdup(epoch->valuestring);
    if (tle->epoch == NULL) {
        LOG(ERROR, "Failed to allocate memory for EPOCH");
        return ERROR;
    }

    // CLASSIFICATION_TYPE
    const cJSON *classification_type = cJSON_GetObjectItemCaseSensitive(omm_json, "CLASSIFICATION_TYPE");
    if (!cJSON_IsString(classification_type)) {
        LOG(ERROR, "Failed to load CLASSIFICATION_TYPE");
        return ERROR;
    }
    tle->classification_type = strdup(classification_type->valuestring);
    if (tle->classification_type == NULL) {
        LOG(ERROR, "Failed to allocate memory for CLASSIFICATION_TYPE");
        return ERROR;
    }

    // NORAD_CAT_ID
    const cJSON *norad_cat_id = cJSON_GetObjectItemCaseSensitive(omm_json, "NORAD_CAT_ID");
    if (!cJSON_IsString(norad_cat_id)) {
        LOG(ERROR, "Failed to load NORAD_CAT_ID");
        return ERROR;
    }
    tle->norad_cat_id = strdup(norad_cat_id->valuestring);
    if (tle->norad_cat_id == NULL) {
        LOG(ERROR, "Failed to allocate memory for NORAD_CAT_ID");
        return ERROR;
    }

    // OBJECT_TYPE
    const cJSON *object_type = cJSON_GetObjectItemCaseSensitive(omm_json, "OBJECT_TYPE");
    if (!cJSON_IsString(object_type)) {
        LOG(ERROR, "Failed to load OBJECT_TYPE");
        return ERROR;
    }
    tle->object_type = strdup(object_type->valuestring);
    if (tle->object_type == NULL) {
        LOG(ERROR, "Failed to allocate memory for OBJECT_TYPE");
        return ERROR;
    }

    // RCS_SIZE
    const cJSON *rcs_size = cJSON_GetObjectItemCaseSensitive(omm_json, "RCS_SIZE");
    if (!cJSON_IsString(rcs_size)) {
        LOG(ERROR, "Failed to load RCS_SIZE");
        return ERROR;
    }
    tle->rcs_size = strdup(rcs_size->valuestring);
    if (tle->rcs_size == NULL) {
        LOG(ERROR, "Failed to allocate memory for RCS_SIZE");
        return ERROR;
    }

    // COUNTRY_CODE
    const cJSON *country_code = cJSON_GetObjectItemCaseSensitive(omm_json, "COUNTRY_CODE");
    if (!cJSON_IsString(country_code)) {
        LOG(ERROR, "Failed to load COUNTRY_CODE");
        return ERROR;
    }
    tle->country_code = strdup(country_code->valuestring);
    if (tle->country_code == NULL) {
        LOG(ERROR, "Failed to allocate memory for COUNTRY_CODE");
        return ERROR;
    }

    // LAUNCH_DATE
    const cJSON *launch_date = cJSON_GetObjectItemCaseSensitive(omm_json, "LAUNCH_DATE");
    if (!cJSON_IsString(launch_date)) {
        LOG(ERROR, "Failed to load LAUNCH_DATE");
        return ERROR;
    }
    tle->launch_date = strdup(launch_date->valuestring);
    if (tle->launch_date == NULL) {
        LOG(ERROR, "Failed to allocate memory for LAUNCH_DATE");
        return ERROR;
    }

    // SITE
    const cJSON *site = cJSON_GetObjectItemCaseSensitive(omm_json, "SITE");
    if (!cJSON_IsString(site)) {
        LOG(ERROR, "Failed to load SITE");
        return ERROR;
    }
    tle->site = strdup(site->valuestring);
    if (tle->site == NULL) {
        LOG(ERROR, "Failed to allocate memory for SITE");
        return ERROR;
    }

    // DECAY_DATE
    const cJSON *decay_date = cJSON_GetObjectItemCaseSensitive(omm_json, "DECAY_DATE");
    if (!cJSON_IsString(decay_date) && !cJSON_IsNull(decay_date)) {
        LOG(ERROR, "Failed to load DECAY_DATE");
        return ERROR;
    }
    if (cJSON_IsString(decay_date)) {
        tle->decay_date = strdup(decay_date->valuestring);
    } else {
        tle->decay_date = "0000-00-00";
    }
    if (tle->decay_date == NULL) {
        LOG(ERROR, "Failed to allocate memory for DECAY_DATE");
        return ERROR;
    }
    
    // EPHEMERIS_TYPE
    const cJSON *ephemeris_type = cJSON_GetObjectItemCaseSensitive(omm_json, "EPHEMERIS_TYPE");
    if (!cJSON_IsString(ephemeris_type)) {
        LOG(ERROR, "Failed to load EPHEMERIS_TYPE");
        return ERROR;
    }
    tle->ephemeris_type = atoi(ephemeris_type->valuestring);

    // ELEMENT_SET_NO
    const cJSON *element_set_no = cJSON_GetObjectItemCaseSensitive(omm_json, "ELEMENT_SET_NO");
    if (!cJSON_IsString(element_set_no)) {
        LOG(ERROR, "Failed to load ELEMENT_SET_NO");
        return ERROR;
    }
    tle->element_set_no = atoi(element_set_no->valuestring);

    // REV_AT_EPOCH
    const cJSON *rev_at_epoch = cJSON_GetObjectItemCaseSensitive(omm_json, "REV_AT_EPOCH");
    if (!cJSON_IsString(rev_at_epoch)) {
        LOG(ERROR, "Failed to load REV_AT_EPOCH");
        return ERROR;
    }
    tle->rev_at_epoch = atoi(rev_at_epoch->valuestring);

    // FILE
    const cJSON *file = cJSON_GetObjectItemCaseSensitive(omm_json, "FILE");
    if (!cJSON_IsString(file)) {
        LOG(ERROR, "Failed to load FILE");
        return ERROR;
    }
    tle->file = atoi(file->valuestring);

    // GP_ID
    const cJSON *gp_id = cJSON_GetObjectItemCaseSensitive(omm_json, "GP_ID");
    if (!cJSON_IsString(gp_id)) {
        LOG(ERROR, "Failed to load GP_ID");
        return ERROR;
    }
    tle->gp_id = atoi(gp_id->valuestring);

    // MEAN_MOTION
    const cJSON *mean_motion = cJSON_GetObjectItemCaseSensitive(omm_json, "MEAN_MOTION");
    if (!cJSON_IsString(mean_motion)) {
        LOG(ERROR, "Failed to load MEAN_MOTION");
        return ERROR;
    }
    tle->mean_motion = atoi(mean_motion->valuestring);

    // ECCENTRICITY
    const cJSON *eccentricity = cJSON_GetObjectItemCaseSensitive(omm_json, "ECCENTRICITY");
    if (!cJSON_IsString(eccentricity)) {
        LOG(ERROR, "Failed to load ECCENTRICITY");
        return ERROR;
    }
    tle->eccentricity = atoi(eccentricity->valuestring);

    // INCLINATION
    const cJSON *inclination = cJSON_GetObjectItemCaseSensitive(omm_json, "INCLINATION");
    if (!cJSON_IsString(inclination)) {
        LOG(ERROR, "Failed to load INCLINATION");
        return ERROR;
    }
    tle->inclination = atoi(inclination->valuestring);

    // RA_OF_ASC_NODE
    const cJSON *ra_of_asc_node = cJSON_GetObjectItemCaseSensitive(omm_json, "RA_OF_ASC_NODE");
    if (!cJSON_IsString(ra_of_asc_node)) {
        LOG(ERROR, "Failed to load RA_OF_ASC_NODE");
        return ERROR;
    }
    tle->ra_of_asc_node = atoi(ra_of_asc_node->valuestring);

    // ARG_OF_PERICENTER
    const cJSON *arg_of_pericenter = cJSON_GetObjectItemCaseSensitive(omm_json, "ARG_OF_PERICENTER");
    if (!cJSON_IsString(arg_of_pericenter)) {
        LOG(ERROR, "Failed to load ARG_OF_PERICENTER");
        return ERROR;
    }
    tle->arg_of_pericenter = atoi(arg_of_pericenter->valuestring);

    // MEAN_ANOMALY
    const cJSON *mean_anomaly = cJSON_GetObjectItemCaseSensitive(omm_json, "MEAN_ANOMALY");
    if (!cJSON_IsString(mean_anomaly)) {
        LOG(ERROR, "Failed to load MEAN_ANOMALY");
        return ERROR;
    }
    tle->mean_anomaly = atoi(mean_anomaly->valuestring);

    // BSTAR
    const cJSON *bstar = cJSON_GetObjectItemCaseSensitive(omm_json, "BSTAR");
    if (!cJSON_IsString(bstar)) {
        LOG(ERROR, "Failed to load BSTAR");
        return ERROR;
    }
    tle->bstar = atoi(bstar->valuestring);

    // MEAN_MOTION_DOT
    const cJSON *mean_motion_dot = cJSON_GetObjectItemCaseSensitive(omm_json, "MEAN_MOTION_DOT");
    if (!cJSON_IsString(mean_motion_dot)) {
        LOG(ERROR, "Failed to load MEAN_MOTION_DOT");
        return ERROR;
    }
    tle->mean_motion_dot = atoi(mean_motion_dot->valuestring);

    // MEAN_MOTION_DDOT
    const cJSON *mean_motion_ddot = cJSON_GetObjectItemCaseSensitive(omm_json, "MEAN_MOTION_DDOT");
    if (!cJSON_IsString(mean_motion_ddot)) {
        LOG(ERROR, "Failed to load MEAN_MOTION_DDOT");
        return ERROR;
    }
    tle->mean_motion_ddot = atoi(mean_motion_ddot->valuestring);

    // SEMIMAJOR_AXIS
    const cJSON *semimajor_axis = cJSON_GetObjectItemCaseSensitive(omm_json, "SEMIMAJOR_AXIS");
    if (!cJSON_IsString(semimajor_axis)) {
        LOG(ERROR, "Failed to load SEMIMAJOR_AXIS");
        return ERROR;
    }
    tle->semimajor_axis = atoi(semimajor_axis->valuestring);

    // PERIOD
    const cJSON *period = cJSON_GetObjectItemCaseSensitive(omm_json, "PERIOD");
    if (!cJSON_IsString(period)) {
        LOG(ERROR, "Failed to load PERIOD");
        return ERROR;
    }
    tle->period = atoi(period->valuestring);

    // APOAPSIS
    const cJSON *apoapsis = cJSON_GetObjectItemCaseSensitive(omm_json, "APOAPSIS");
    if (!cJSON_IsString(apoapsis)) {
        LOG(ERROR, "Failed to load APOAPSIS");
        return ERROR;
    }
    tle->apoapsis = atoi(apoapsis->valuestring);

    // PERIAPSIS
    const cJSON *periapsis = cJSON_GetObjectItemCaseSensitive(omm_json, "PERIAPSIS");
    if (!cJSON_IsString(periapsis)) {
        LOG(ERROR, "Failed to load PERIAPSIS");
        return ERROR;
    }
    tle->periapsis = atoi(periapsis->valuestring);

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
