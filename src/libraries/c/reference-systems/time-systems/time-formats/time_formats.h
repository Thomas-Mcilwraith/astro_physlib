#ifndef TIME_FORMATS
#define TIME_FORMATS

// Standard libraries
#include <math.h>

// Local libraries
#include "utilities/logging/log/log.h"
#include "external/sofa/sofa.h"

// Variable and Macro definitions

// Function prototypes

/**
 * @brief Converts calendar date into Julian Day.
 * 
 * @param julian_day (*double) Julian Day [4713 B.C.]
 * @param year Year (between 1900 -> 2100)
 * @param month Month of Year
 * @param day Day of Month
 * @param hour Hour of Day
 * @param minute Minute of Hour
 * @param second Second+Millisecond of Minute
 * 
 */
StatusCode date_to_jd(
        // Outputs
        double *julian_day,
        // Inputs
        const int year, const int month, const int day,
        const int hour, const int minute, const double second);


/**
 * @brief Converts Julian Day into calendar date.
 * 
 * @param year Year (between 1900 -> 2100)
 * @param month Month of Year
 * @param day Day of Month
 * @param hour Hour of Day
 * @param minute Minute of Hour
 * @param second Second+Millisecond of Minute
 * @param julian_day (*double) Julian Day [4713 B.C.]
 */
StatusCode jd_to_date(
        // Outputs
        int *year, int *month, int *day,
        int *hour, int *minute, double *second,
        // Inputs
        const double julian_day
    );

/**
 * Returns the day of the year for a given date.
 *
 * @param year Year
 * @param month Month of Year
 * @param day Day of Month
 *
 * @return Day of Year
 */
int day_of_year(int year, int month, int day);

/**
 * @brief
 * Returns the day of the week for a given Julian Day
 * 
 * @param julian_day The Julian Day
 * @return The day of the week 1 -> 7 (Monday -> Sunday)
 */
int day_of_week(double julian_day);

/**
 * @brief Converts Julian Day to Modified Julian Date.
 * 
 * @param julian_day (*double) Julian Day [4713 B.C.]
 * 
 * @return Modified Julian Date
 */
double jd_to_mjd(const double julian_day);

/**
 * @brief Converts Julian Day to Modified Julian Date (2000).
 * 
 * @param julian_day (*double) Julian Day [4713 B.C.]
 * 
 * @return Modified Julian Date (2000)
 */
double jd_to_mjd2000(const double julian_day);

/**
 * @brief Converts Julian Day to Modified Julian Date (1900).
 * 
 * @param julian_day (*double) Julian Day [4713 B.C.]
 * 
 * @return Modified Julian Date (1900)
 */
double jd_to_mjd1900(const double julian_day);

/**
 * @brief Converts Julian Day to Modified Julian Date (GPS).
 * 
 * @param julian_day (*double) Julian Day [4713 B.C.]
 * 
 * @return Modified Julian Date (GPS)
 */
double jd_to_mjd1900(const double julian_day);

/**
 * @brief Converts Julian Day to GPS Epoch.
 * 
 * @param julian_day (*double) Julian Day [4713 B.C.]
 * 
 * @return Julian Century
 */
double jd_to_mjdGPS(const double julian_day);

/**
 * @brief Converts Julian Day to Julian Century.
 * 
 * @param julian_day (*double) Julian Day [4713 B.C.]
 * 
 * @return Julian Century
 */
double jd_to_jc(const double julian_day);

#endif