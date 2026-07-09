#ifndef TIME_FORMATS
#define TIME_FORMATS

// Standard libraries
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Local libraries
#include "utilities/logging/log/log.h"
#include "external/sofa/sofa.h"

// Variable and Macro definitions

// Function prototypes
typedef struct {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    double second;
} datetime_t;

/**
 * @brief Converts calendar date into Julian Day.
 * 
 * @param julian_day (*double) Julian Day [4713 B.C.]
 * @param date Gregorian calendar date/time
 * 
 */
StatusCode date_to_jd(
        // Outputs
        double *julian_day,
        // Inputs
        const datetime_t date);


/**
 * @brief Converts Julian Day into calendar date.
 * 
 * @param date Gregorian calendar date/time
 * @param julian_day (*double) Julian Day [4713 B.C.]
 */
StatusCode jd_to_date(
        // Outputs
        datetime_t *date,
        // Inputs
        const double julian_day);

/**
 * Returns the day of the year for a given date.
 *
 * @param date Gregorian calendar date/time
 * @return Day of Year
 */
int day_of_year(const datetime_t date);

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

/**
 * @brief Converts ISO8601 date/time string to calendar date/time.
 * 
 * @param date Gregorian calendar date/time
 * @param iso8601 ISO8601 date/time string
 */
StatusCode iso8601_to_date(
        // Outputs
        datetime_t *date,
        bool *is_utc,
        // Inputs
        const char *iso8601);

/**
 * @brief Converts calendar date/time to ISO8601 date/time string.
 * 
 * @param iso8601 ISO8601 date/time string
 * @param date Gregorian calendar date/time
 * @param is_utc (*bool) True if date/time is UTC
 */
StatusCode date_to_iso8601(
        // Outputs
        char *iso8601,
        // Inputs
        const datetime_t date,
        const bool is_utc);
#endif
