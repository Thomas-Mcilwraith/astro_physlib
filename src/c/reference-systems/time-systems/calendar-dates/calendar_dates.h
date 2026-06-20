#ifndef CALENDAR_DATES_H
#define CALENDAR_DATES_H

// Standard libraries
#include <math.h>
#include <stdbool.h>

// Local libraries
#include "utilities/logging/log/log.h"

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

#endif
