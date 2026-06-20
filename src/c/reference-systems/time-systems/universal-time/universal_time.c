/*
 * universal_time.c
 *
 * Author: Thomas McIlwraith
 * Date: 20/06/2026
 */

#include "universal_time.h"

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

double ut0(double hour_angle, double longitude, bool is_greenwich) {
    if (is_greenwich) {
        return 12 + hour_angle;
    } else {
        return 12 + hour_angle - angle_to_hour_angle(longitude);
    }
}

double ut0_to_ut1(double ut0, double xp, double yp, double longitude,
                  double geocentric_latitude) {
    return ut0+(xp*sin(longitude)+yp*cos(longitude))*tan(geocentric_latitude);
}

double angle_to_hour_angle(double angle) {
    return angle/DEG_TO_RAD*15;
}
