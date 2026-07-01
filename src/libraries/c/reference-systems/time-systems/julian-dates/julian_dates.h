#ifndef JULIAN_DATES_H
#define JULIAN_DATES_H

// Standard libraries

// Local libraries

// Variable and Macro definitions

// Function prototypes

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
