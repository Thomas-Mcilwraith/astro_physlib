/*
 * julian_dates.c
 *
 * Author: Thomas McIlwraith
 * Date: 18/04/2026
 */

#include "julian_dates.h"

double jd_to_mjd(const double julian_day) {
    return julian_day - 2400000.5;
}

double jd_to_mjd2000(const double julian_day) {
    return julian_day - 2451545.0;
}

double jd_to_mjd1900(const double julian_day) {
    return julian_day - 2415021.0;
}

double jd_to_mjdGPS(const double julian_day) {
    return julian_day - 2444244.5;
}

double jd_to_jc(const double julian_day) {
    return julian_day/36525.0;
}
