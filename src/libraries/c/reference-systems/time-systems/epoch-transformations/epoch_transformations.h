#ifndef EPOCH_TRANSFORMATIONS_H
#define EPOCH_TRANSFORMATIONS_H

// Standard libraries
#include <math.h>
#include <stdbool.h>

// Local libraries
#include "utilities/constants/constants.h"

// Variable and Macro definitions

// Function prototypes

/**
 * @brief
 * Converts Julian Centuries since Jan 0 1900 @ Greenwich to Fictitious Mean
 * Sun (FMS)
 * 
 * @param jc1900 The Julian Centuries since Jan 0 1900 @ Greenwich
 * @return Fictitious Mean Sun
 */
double jc1900_to_FMS(const double jc1900);

/**
 * @brief
 * Calculates the equation of time
 * 
 * @param sun_mean_anomaly The sun mean anomaly
 * @param obliquity_of_ecliptic The obliquity of the ecliptic
 * @return Equation of time
 */
double eq_of_time(const double sun_mean_anomaly, const double obliquity_of_ecliptic);

/**
 * @brief
 * Converts jd_utc to jd_ut1
 *
 * @note
 * UT1-UTC is retrieved from Online EOP sources.
 * 
 * @param jd_utc Julian Day
 * @param ut1_minus_utc_seconds UT1 - UTC in seconds
 * @return jd_ut1
 */
double utc_to_ut1(const double jd_utc, const double ut1_minus_utc_seconds);

/**
 * @brief
 * Converts an angle in radians to an hour angle
 * 
 * @param angle The angle in radians
 * @return The hour angle
 */
double angle_to_hour_angle(const double angle);



#endif
