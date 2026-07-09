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
        const datetime_t date) {

    if (date.year<0 || date.month<0 || date.day<0 || date.hour<0 || date.minute<0 || date.second<0.0) {
        LOG(ERROR, "Negative date elements in date_to_jd");
        LOG(INFO, "%d-%d-%dT%d:%d:%f",
            date.year, date.month, date.day, date.hour, date.minute, date.second);
        return ERROR;
    }

    // Always perform the computation, even outwith the span.
    *julian_day = (
        367 * date.year - (int)(7*(date.year + (int)(((double)(date.month)+9)/12))/4) +
        (int)(275*date.month/9) + date.day + 1721013.5 +
        ((date.second/60 + date.minute)/60 + date.hour)/24
    );

    if (date.year < 1900 || date.year > 2100) {
        LOG(WARNING, "Date outwith valid period for date -> JD conversion "
                       "(1900 < date < 2100).");
        return WARNING;
    }

    return OK;
}


StatusCode jd_to_date(
        // Outputs
        datetime_t *date,
        // Inputs
        const double julian_day) {

    if (julian_day <= 0.0) {
        LOG(ERROR, "Negative Julian Day in jd_to_date");
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

    date->day = b - 2447 * j / 80;

    int l = j / 11;
    date->month = j + 2 - 12 * l;
    date->year = 100 * (n - 49) + i + l;

    double total_seconds = fractional_day * 86400.0;

    date->hour = (int)(total_seconds / 3600.0);
    total_seconds -= date->hour * 3600.0;

    date->minute = (int)(total_seconds / 60.0);
    date->second = total_seconds - date->minute * 60.0;
    
    return OK;
}

int day_of_week(double julian_day) {
    return floor(julian_day - 7*floor((julian_day+1)/7) + 2);
}

int day_of_year(const datetime_t date) {

    // Local variables
    int day_of_year = 0;
    int days_in_months[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Leap year check
    if ((date.year%4 == 0 && date.year%100 != 0) || (date.year%400 == 0)) {
        days_in_months[1] = 29;
    }

    for (int i=1; i<date.month; i++) {
        day_of_year += days_in_months[i-1];
    }

    day_of_year += date.day;

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

StatusCode iso8601_to_date(
        // Outputs
        datetime_t *date,
        bool *is_utc,
        // Inputs
        const char *iso8601) {

    // Local variables
    const int iso8601_len = (int)strlen(iso8601);

    if (date == NULL || is_utc == NULL || iso8601 == NULL) {
        LOG(ERROR, "Bad pointers to iso8601_to_date");
        return ERROR;
    }

    // Check for ZULU time (UTC) Identifier
    if (iso8601_len == 23) {
        *is_utc = false;
    } else if (iso8601_len == 24 && iso8601[23] == 'Z') {
        *is_utc = true;
    } else {
        LOG(ERROR, "Invalid ISO8601 date format: %s", iso8601);
        LOG(INFO, "Expected format: YYYY-MM-DDTHH:MM:SS.sss(Z)");
        return ERROR;
    }

    // Check seperators
    if (iso8601[4] != '-' || iso8601[7] != '-' || iso8601[10] != 'T' ||
        iso8601[13] != ':' || iso8601[16] != ':') {
        LOG(ERROR, "Invalid ISO8601 date format: %s", iso8601);
        LOG(INFO, "Expected format: YYYY-MM-DDTHH:MM:SS.sss(Z)");
        return ERROR;
    }

    // Parse date
    date->year = atoi(iso8601+0);
    date->month = atoi(iso8601+5);
    date->day = atoi(iso8601+8);
    date->hour = atoi(iso8601+11);
    date->minute = atoi(iso8601+14);
    date->second = atof(iso8601+17);

    // Check for valid date
    // Allow 61 seconds for leap seconds, in some applications
    if (date->month < 1 || date->month > 12 ||
        date->day < 1 || date->day > 31 ||
        date->hour < 0 || date->hour > 23 ||
        date->minute < 0 || date->minute > 59 ||
        date->second < 0 || date->second > 61.0) {
        LOG(ERROR, "Invalid date value(s) in iso8601_to_date: %s", iso8601);
        LOG(INFO, "Expected format: YYYY-MM-DDTHH:MM:SS.sss(Z)");
        return ERROR;
    }

    return OK;
}

StatusCode date_to_iso8601(
        // Outputs
        char *iso8601,
        // Inputs
        const datetime_t date,
        const bool is_utc) {

    if (iso8601 == NULL) {
        LOG(ERROR, "Bad pointer in date_to_iso8601");
        return ERROR;
    }

    if (is_utc) {
        snprintf(iso8601, 25, "%04d-%02d-%02dT%02d:%02d:%06.3fZ",
                 date.year, date.month, date.day, date.hour, date.minute, date.second);
    } else {
        snprintf(iso8601, 24, "%04d-%02d-%02dT%02d:%02d:%06.3f",
                 date.year, date.month, date.day, date.hour, date.minute, date.second);
    }

    return OK;
}
