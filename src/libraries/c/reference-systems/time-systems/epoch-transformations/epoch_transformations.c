/*
 * epoch_transformations.c
 *
 * Author: Thomas McIlwraith
 * Date: 20/06/2026
 */

#include "epoch_transformations.h"

double jc1900_to_FMS(double jc1900) {
    return (18*60*60)*(38*60)*45.836+8640184.542*jc1900+0.0929*jc1900*jc1900;
}

double eq_of_time(double sun_mean_anomaly, double obliquity_of_ecliptic) {
    return (-1.91466471*DEG_TO_RAD*sin(sun_mean_anomaly) -
            0.01999643*sin(2*sun_mean_anomaly) +
            2.466*sin(2*obliquity_of_ecliptic) -
            0.0053*sin(4*obliquity_of_ecliptic)
           );
}

double utc_to_ut1(double jd_utc, double ut1_minus_utc_seconds) {
    return jd_utc + ut1_minus_utc_seconds/SECONDS_PER_DAY;
}

double angle_to_hour_angle(double angle) {
    return angle/DEG_TO_RAD*15;
}

StatusCode utc_to_tai(
    // Outputs
    double *tai_jd,
    // Inputs
    const double utc_jd) {

    // Local variables
    double tai1, tai2;
    int sofa_status;

    sofa_status = iauUtctai(utc_jd, 0.0, &tai1, &tai2);
    if (sofa_status == -1) {
        LOG(ERROR, "Bad date passed to UTC->TAI conversion");
        return ERROR;
    }

    // Output JD is sum of SOFA JD parts
    *tai_jd = tai1 + tai2;

    if (sofa_status == 1) {
        LOG(WARNING, "Dubious year detected. Input UTC is too late/early to "
                       "accurately convert to TAI;");
        return WARNING;
    }

    return OK;
}

/**
 * @brief Converts TAI to Terrestrial time (TT)
 * 
 * @param tai_jd TAI time (julian days)
 */
double tai_to_tt(const double tai_jd) {

    // Local variables
    double tt1, tt2;

    iauTaitt(tai_jd, 0.0, &tt1, &tt2);
    return tt1 + tt2;
}