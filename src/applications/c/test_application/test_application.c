/*
 * test_application.c
 *
 * Author: Thomas McIlwraith
 * Date: 03/07/2026
 * 
 * Computes an example conversion from GCRF to ITRF.
 * 
 */

#include "utilities/misc/parse-cmdline/parse_cmdline.h"
#include "utilities/logging/log/log.h"
#include "reference-systems/coordinate-systems/terrestrial-frames/terrestrial_frames.h"
#include "reference-systems/time-systems/time-formats/time_formats.h"
#include "reference-systems/time-systems/epoch-transformations/epoch_transformations.h"
#include "mathematics-library/linear-algebra/matrix-operations/matrix_operations.h"
#include "external/sofa/sofa.h"
#include "test_application_interfaces.h"

int main(int argc, char *argv[]) {
    // Program configuration
    const char *program_name = "test_application";
    TestApplicationInputs main_options;
    ExecutionSettings execution_settings;

    // Program variables
    StatusCode status = OK;
    double utc_jd, ut1_jd, tt_jd, tai_jd, tt_mjd2000;
    double tio_locator, earth_rotation_angle;
    double R_gcrs_tirs[3][3], R_cirs_tirs[3][3], R_gcrs_cirs[3][3], R_tirs_itrs[3][3], R_gcrs_itrs[3][3];
    double itrf_vec[3];
    double output_vec[3];
    datetime_t date;

    status = parse_cmdline(&execution_settings, argc, argv);
    if (status != OK) {
        LOG(ERROR, "Failed to parse command line");
        return ERROR;
    }

    init_log(execution_settings.run_title, execution_settings.working_directory, program_name);

    status = read_TestApplicationInputs(&main_options, execution_settings.working_directory,
                                        execution_settings.run_title, program_name);
    if (status != OK) {
        LOG(ERROR, "Failed to read Inputs");
        return ERROR;
    }

    // Unpack the main options [WILL NOT DO THIS IN REAL APPICATIONS]
    date.year = main_options.year;
    date.month = main_options.month;
    date.day = main_options.day;
    date.hour = main_options.hour;
    date.minute = main_options.minute;
    date.second = main_options.seconds;
    const double gcrf_vec[3] = {main_options.gcrf_vec[0], main_options.gcrf_vec[1], main_options.gcrf_vec[2]};
    const double utc_ut1_sec = main_options.utc_ut1_sec;
    const double xp = main_options.xp;
    const double yp = main_options.yp;
    const double dx_CIP = main_options.dx_CIP;
    const double dy_CIP = main_options.dy_CIP;


    LOG(INFO, "Computing GCRF -> ITRF Rotation");
    LOG(INFO, "Date: %04d-%02d-%02d %02d:%02d:%06.3f UTC", date.year, date.month, date.day, date.hour, date.minute, date.second);
    LOG(INFO, "GCRF Input vec : %f    %f    %f", gcrf_vec[0], gcrf_vec[1], gcrf_vec[2]);

    status = date_to_jd(&utc_jd, date);
    if (status != OK) {
        LOG(ERROR, "Failed to compute JD from date");
        return ERROR;
    }
    LOG(INFO, "Julian day (UTC) is: %.10f", utc_jd);

    ut1_jd = utc_to_ut1(utc_jd, utc_ut1_sec);
    LOG(INFO, "Julian day (UT1) is: %.10f", ut1_jd);

    status = utc_to_tai(&tai_jd, utc_jd);
    if (status == ERROR) {
        LOG(ERROR, "Failed to convert UTC to TAI");
        return ERROR;
    }
    LOG(INFO, "Julian day (TAI) is: %.10f", tai_jd);

    tt_jd = tai_to_tt(tai_jd);
    LOG(INFO, "Julian day (TT) is: %.10f", tt_jd);
    tt_mjd2000 = jd_to_mjd2000(tt_jd);
    LOG(INFO, "Modified Julian day 2000 (TT) is: %.10f", tt_mjd2000);

    tio_locator = get_tio_locator(tt_jd);
    LOG(INFO, "TIO locator is: %.16f", tio_locator);

    earth_rotation_angle = get_earth_rotation_angle(ut1_jd);
    LOG(INFO, "Earth Rotation Angle is: %.10f", earth_rotation_angle);

    status = rotmat_gcrs_to_cirs(R_gcrs_cirs, tt_mjd2000, 2, dx_CIP, dy_CIP, false);
    if (status != OK) {
        LOG(ERROR, "Failed to compute rotation CIRS -> GCRS");
        return ERROR;
    }

    status = rotmat_cirs_to_tirs(R_cirs_tirs, earth_rotation_angle, false);
    if (status != OK) {
        LOG(ERROR, "Failed to compute rotation TIRS -> CIRS");
        return ERROR;
    }

    status = rotmat_tirs_to_itrs(R_tirs_itrs, xp, yp, tio_locator, false);
    if (status != OK) {
        LOG(ERROR, "Failed to compute rotation ITRS -> TIRS");
        return ERROR;
    }

    // Assemble rotations
    mat3_mul(R_gcrs_tirs, R_cirs_tirs, R_gcrs_cirs);
    mat3_mul(R_gcrs_itrs, R_tirs_itrs, R_gcrs_tirs);

    // Final results(s)
    vec3_rotate(output_vec, R_gcrs_itrs, gcrf_vec);
    LOG(INFO, "ITRF Output vec: %f    %f    %f", output_vec[0], output_vec[1], output_vec[2]);

    LOG(INFO, "Performing comparison with iauC2t06a method");
    double iauR_gcrs_itrs[3][3];
    double iau_output_vec[3];
    iauC2t06a(tt_jd, 0.0, ut1_jd, 0.0, xp, yp, iauR_gcrs_itrs);
    vec3_rotate(iau_output_vec, iauR_gcrs_itrs, gcrf_vec);
    LOG(INFO, "ITRF Output vec (iau): %f    %f    %f", output_vec[0], output_vec[1], output_vec[2]);
    LOG(INFO, "RMS difference with IAU: %.10f", fabs(vec3_norm(iau_output_vec) - vec3_norm(output_vec)));

    // Free memory
    LOG(INFO, "Program complete: %s %s", program_name, execution_settings.run_title);
    free_TestApplicationInputs(&main_options);
    close_log();
    return OK;
}
