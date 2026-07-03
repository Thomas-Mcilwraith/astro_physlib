#include "utilities/logging/log/log.h"
#include "reference-systems/time-systems/time-formats/time_formats.h"
#include "reference-systems/time-systems/epoch-transformations/epoch_transformations.h"

int main() {
    init_log();

    // Local variables
    const int year = 2019;
    const int month = 1;
    const int day = 4;
    const int hour = 12;
    const int minute = 0;
    const double seconds = 0.0;
    const double ut1_utc = -0.0417339;
    StatusCode status = OK;
    double utc_jd, ut1_jd;

    status = date_to_jd(&utc_jd, year, month, day, hour, minute, seconds);
    if (status != OK) {
        LOG("ERROR", "Failed to compute JD from date.");
        return ERROR;
    }

    ut1_jd = utc_to_ut1(utc_jd, ut1_utc);

    

    return OK;
}
