#ifndef UNIVERSAL_TIME_H
#define UNIVERSAL_TIME_H

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
double jc1900_to_FMS(double jc1900);

/**
 * @brief
 * Calculates the equation of time
 * 
 * @param sun_mean_anomaly The sun mean anomaly
 * @param obliquity_of_ecliptic The obliquity of the ecliptic
 * @return Equation of time
 */
double eq_of_time(double sun_mean_anomaly, double obliquity_of_ecliptic);

/**
 * @brief
 * Calculates the universal time at Greenwich.
 *
 * @note
 * The longitude is in radians. The output is in hours.
 * 
 * @param hour_angle The hour angle
 * @param longitude The longitude, measure in radians
 * @param is_greenwich Whether the time is Greenwich or Local
 * @return UT0
 */
double ut0(double hour_angle, double longitude, bool is_greenwich);

/**
 * @brief
 * Determine UT1 from UT0
 *
 * @note
 * Angles are in radians.
 * 
 * @param ut0 The universal time at Greenwich
 * @param xp The x-coordinate of the pole
 * @param yp The y-coordinate of the pole
 * @param longitude The longitude of the observer, measure in radians
 * @param geocentric_latitude The geocentric latitude of the observer, measure in radians
 * @return UT1
 */
double ut0_to_ut1(double ut0, double xp, double yp, double longitude,
                  double geocentric_latitude);

/**
 * @brief
 * Converts an angle in radians to an hour angle
 * 
 * @param angle The angle in radians
 * @return The hour angle
 */
double angle_to_hour_angle(double angle);
#endif

/**
 * @brief
 * Returns the day of the week for a given Julian Day
 * 
 * @param julian_day The Julian Day
 * @return The day of the week 1 -> 7 (Monday -> Sunday)
 */
int day_of_week(double julian_day);
