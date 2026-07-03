#include "utilities/logging/log/log.h"
#include "reference-systems/coordinate-systems/terrestrial-frames/terrestrial_frames.h"
#include "reference-systems/time-systems/time-formats/time_formats.h"
#include "reference-systems/time-systems/epoch-transformations/epoch_transformations.h"
#include "mathematics-library/linear-algebra/matrix-operations/matrix_operations.h"

int main() {
    init_log();

    // Local variables
    const int year = 2019;
    const int month = 1;
    const int day = 4;
    const int hour = 12;
    const int minute = 0;
    const double seconds = 0.0;
    const double utc_ut1_sec = -0.0417339;
    const double xp_arcsec = 0.068294;
    const double yp_arcsec = 0.277004;
    const double dx_CIP = 0.426 / 1000 * ARCSEC_TO_RAD;
    const double dy_CIP = 0.170 / 1000 * ARCSEC_TO_RAD;
    StatusCode status = OK;
    double utc_jd, ut1_jd, tt_jd, tai_jd, tt_mjd2000;
    double tio_locator, earth_rotation_angle;
    double R_itrs_tirs[3][3], R_tirs_cirs[3][3], R_cirs_gcrs[3][3], R_itrs_gcrs[3][3];
    double R_temp1[3][3], R_temp2[3][3];

    status = date_to_jd(&utc_jd, year, month, day, hour, minute, seconds);
    if (status != OK) {
        LOG("ERROR", "Failed to compute JD from date");
        return ERROR;
    }
    LOG("INFO", "Julian day (UTC) is: %.10f", utc_jd);

    ut1_jd = utc_to_ut1(utc_jd, utc_ut1_sec);
    LOG("INFO", "Julian day (UT1) is: %.10f", ut1_jd);

    status = utc_to_tai(&tai_jd, utc_jd);
    if (status == ERROR) {
        LOG("ERROR", "Failed to convert UTC to TAI");
        return ERROR;
    }
    LOG("INFO", "Julian day (TAI) is: %.10f", tai_jd);

    tt_jd = tai_to_tt(tai_jd);
    LOG("INFO", "Julian day (TT) is: %.10f", tt_jd);
    tt_mjd2000 = jd_to_mjd2000(tt_jd);
    LOG("INFO", "Modified Julian day 2000 (TT) is: %.10f", tt_mjd2000);

    tio_locator = get_tio_locator(tt_jd);
    LOG("INFO", "TIO locator is: %.16f", tio_locator);

    status = rotmat_itrs_to_tirs(R_itrs_tirs, xp_arcsec*ARCSEC_TO_RAD,
                                 yp_arcsec*ARCSEC_TO_RAD, tio_locator, false);
    if (status != OK) {
        LOG("ERROR", "Failed to compute rotation ITRS -> TIRS");
        return ERROR;
    }

    earth_rotation_angle = get_earth_rotation_angle(ut1_jd);
    LOG("INFO", "Earth Rotation Angle is: %.10f", earth_rotation_angle);

    status = rotmat_tirs_to_cirs(R_tirs_cirs, earth_rotation_angle, false);
    if (status != OK) {
        LOG("ERROR", "Failed to compute rotation TIRS -> CIRS");
        return ERROR;
    }

    status = rotmat_cirs_to_gcrs(R_cirs_gcrs, tt_mjd2000, 2, dx_CIP, dy_CIP, false);
    if (status != OK) {
        LOG("ERROR", "Failed to compute rotation CIRS -> GCRS");
        return ERROR;
    }

    // ITRS -> CIRS rotation
    mat3_mul(R_temp1, R_tirs_cirs, R_itrs_tirs);
    mat3_mul(R_itrs_gcrs, R_cirs_gcrs, R_temp1);

    mat3_transpose(R_temp2, R_itrs_gcrs);

    // Final results(s)
    const double input_vec[3] = {-5762640, -1682738, 3156028};
    double output_vec[3];

    // vec3_rotate(output_vec, R_itrs_gcrs, input_vec);
    vec3_rotate(output_vec, R_temp2, input_vec);
    LOG("INFO", "Input vec : %f    %f    %f", input_vec[0], input_vec[1], input_vec[2]);
    LOG("INFO", "Output vec: %f    %f    %f", output_vec[0], output_vec[1], output_vec[2]);

    return OK;
}
