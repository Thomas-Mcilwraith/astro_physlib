/*
 * time_formats.c
 *
 * Author: Thomas McIlwraith
 * Date: 19/06/2026
 */

#include "time_formats.h"

StatusCode date_to_jd(
        // Outputs
        double *julian_day,
        // Inputs
        const int year, const int month, const int day,
        const int hour, const int minute, const double second) {

    if (year<0 || month<0 || day<0 || hour<0 || minute<0 || second<0.0) {
        LOG("ERROR", "Negative date elements in date_to_jd");
        LOG("INFO", "%d/%d/%d %d:%d:%f",
            year, month, day, hour, minute, second);
        return ERROR;
    }

    // Always perform the computation, even outwith the span.
    *julian_day = (
        367 * year - (int)(7*(year + (int)(((double)(month)+9)/12))/4) +
        (int)(275*month/9) + day + 1721013.5 +
        ((second/60 + minute)/60 + hour)/24
    );

    if (year < 1900 || year > 2100) {
        LOG("WARNING", "Date outwith valid period for date -> JD conversion "
                       "(1900 < date < 2100).");
        return WARNING;
    }

    return OK;
}


StatusCode jd_to_date(
        // Outputs
        int *year, int *month, int *day,
        int *hour, int *minute, double *second,
        // Inputs
        const double julian_day) {

    if (julian_day <= 0.0) {
        LOG("ERROR", "Negative Julian Day in jd_to_date");
        return ERROR;
    }

    int J = (int)floor(julian_day + 0.5);
    double fractional_day = julian_day + 0.5 - J;

    int a = J + 68569;
    int n = 4 * a / 146097;
    int A = a - (146097 * n + 3) / 4;
    int i = 4000 * (A + 1) / 1461001;
    int b = A - 1461 * i / 4 + 31;
    int j = 80 * b / 2447;

    *day = b - 2447 * j / 80;

    int l = j / 11;
    *month = j + 2 - 12 * l;
    *year = 100 * (n - 49) + i + l;

    double total_seconds = fractional_day * 86400.0;

    *hour = (int)(total_seconds / 3600.0);
    total_seconds -= *hour * 3600.0;

    *minute = (int)(total_seconds / 60.0);
    *second = total_seconds - *minute * 60.0;
    
    return OK;
}

int day_of_week(double julian_day) {
    return floor(julian_day - 7*floor((julian_day+1)/7) + 2);
}

int day_of_year(int year, int month, int day) {

    // Local variables
    int day_of_year = 0;
    int days_in_months[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Leap year check
    if ((year%4 == 0 && year%100 != 0) || (year%400 == 0)) {
        days_in_months[1] = 29;
    }

    for (int i=1; i<month; i++) {
        day_of_year += days_in_months[i-1];
    }

    day_of_year += day;

    return day_of_year;
}

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