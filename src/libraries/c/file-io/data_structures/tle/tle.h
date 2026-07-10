#ifndef TLE_H
#define TLE_H

// Standard libraries
#include <stdlib.h>

// Local libraries
#include "external/cjson/cJSON.h"
#include "utilities/logging/log/log.h"

// Variable and Macro definitions

// Function prototypes

typedef struct {
    char* tle_line0;
    char* tle_line1;
    char* tle_line2;
    char* ccsds_omm_vers;
    char* comment;
    char* creation_date;
    char* originator;
    char* object_name;
    char* object_id;
    char* center_name;
    char* ref_frame;
    char* time_system;
    char* mean_element_theory;
    char* epoch;
    char* classification_type;
    char* norad_cat_id;
    char* object_type;
    char* rcs_size;
    char* country_code;
    char* launch_date;
    char* site;
    char* decay_date;
    int ephemeris_type;
    int element_set_no;
    int rev_at_epoch;
    int file;
    int gp_id;
    double mean_motion;
    double eccentricity;
    double inclination;
    double ra_of_asc_node;
    double arg_of_pericenter;
    double mean_anomaly;
    double bstar;
    double mean_motion_dot;
    double mean_motion_ddot;
    double semimajor_axis;
    double period;
    double apoapsis;
    double periapsis;
} tle_t;

StatusCode tle_read_json(
    // Outputs
    tle_t *tle,
    // Inputs
    const cJSON *omm_json);

#endif
