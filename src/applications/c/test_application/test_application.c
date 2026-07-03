#include "utilities/logging/log/log.h"
#include "reference-systems/coordinate-systems/terrestrial-frames/terrestrial_frames.h"
#include "reference-systems/time-systems/time-formats/time_formats.h"
#include "reference-systems/time-systems/epoch-transformations/epoch_transformations.h"
#include "mathematics-library/linear-algebra/matrix-operations/matrix_operations.h"
#include "external/sofa/sofa.h"

int main() {
    // Local variables
    const int year = 2019;
    const int month = 1;
    const int day = 4;
    const int hour = 12;
    const int minute = 0;
    const double seconds = 0.0;
    const double gcrf_vec[3] = {-2981784, 5207055, 3161595};
    // -------------------------------------------------------------------------
    const double utc_ut1_sec = -0.0417339; //                                  |
    const double xp = 0.068294 * ARCSEC_TO_RAD; //                             |
    const double yp = 0.277004 * ARCSEC_TO_RAD; //            FROM EOP DATA    |
    const double dx_CIP = 0.426 / 1000 * ARCSEC_TO_RAD; //                     |
    const double dy_CIP = 0.170 / 1000 * ARCSEC_TO_RAD; //                     |
    // -------------------------------------------------------------------------
    StatusCode status = OK;
    double utc_jd, ut1_jd, tt_jd, tai_jd, tt_mjd2000;
    double tio_locator, earth_rotation_angle;
    double R_gcrs_tirs[3][3], R_cirs_tirs[3][3], R_gcrs_cirs[3][3], R_tirs_itrs[3][3], R_gcrs_itrs[3][3];
    double itrf_vec[3];

    init_log();
    LOG("INFO", "Computing GCRF -> ITRF Rotation");
    LOG("INFO", "Date: %04d-%02d-%02d %02d:%02d:%06.3f UTC", year, month, day, hour, minute, seconds);
    LOG("INFO", "GCRF Input vec : %f    %f    %f", gcrf_vec[0], gcrf_vec[1], gcrf_vec[2]);

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

    earth_rotation_angle = get_earth_rotation_angle(ut1_jd);
    LOG("INFO", "Earth Rotation Angle is: %.10f", earth_rotation_angle);

    status = rotmat_gcrs_to_cirs(R_gcrs_cirs, tt_mjd2000, 2, dx_CIP, dy_CIP, false);
    if (status != OK) {
        LOG("ERROR", "Failed to compute rotation CIRS -> GCRS");
        return ERROR;
    }

    status = rotmat_cirs_to_tirs(R_cirs_tirs, earth_rotation_angle, false);
    if (status != OK) {
        LOG("ERROR", "Failed to compute rotation TIRS -> CIRS");
        return ERROR;
    }

    status = rotmat_tirs_to_itrs(R_tirs_itrs, xp, yp, tio_locator, false);
    if (status != OK) {
        LOG("ERROR", "Failed to compute rotation ITRS -> TIRS");
        return ERROR;
    }

    // Assemble rotations
    mat3_mul(R_gcrs_tirs, R_cirs_tirs, R_gcrs_cirs);
    mat3_mul(R_gcrs_itrs, R_tirs_itrs, R_gcrs_tirs);

    // Final results(s)
    double output_vec[3];

    vec3_rotate(output_vec, R_gcrs_itrs, gcrf_vec);
    LOG("INFO", "ITRF Output vec: %f    %f    %f", output_vec[0], output_vec[1], output_vec[2]);

    LOG("INFO", "Performing comparison with iauC2t06a method");
    double iauR_gcrs_itrs[3][3];
    double iau_output_vec[3];
    iauC2t06a(tt_jd, 0.0, ut1_jd, 0.0, xp, yp, iauR_gcrs_itrs);
    vec3_rotate(iau_output_vec, iauR_gcrs_itrs, gcrf_vec);
    LOG("INFO", "ITRF Output vec (iau): %f    %f    %f", output_vec[0], output_vec[1], output_vec[2]);

    LOG("INFO", "RMS difference with IAU: %.10f", fabs(vec3_norm(iau_output_vec) - vec3_norm(output_vec)));

    LOG("INFO", "Program complete.");
    return OK;
}
