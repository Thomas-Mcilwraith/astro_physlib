/*
 * terrestrial_frames.c
 *
 * Author: Thomas McIlwraith
 * Date: 02/07/2026
 */

#include "terrestrial_frames.h"

StatusCode rotmat_itrs_to_tirs(
    // Outputs
    double output_rotmat[3][3],
    // Inputs
    const double xp,
    const double yp,
    const double tio_locator,
    const bool transpose) {
    
    // Local variables
    double output_rotmatT[3][3];

    mat3_initialise(output_rotmat);

    // Call to SOFA library
    iauPom00(xp, yp, tio_locator, output_rotmat);

    if (transpose) {
        mat3_transpose(output_rotmatT, output_rotmat);
        memcpy(output_rotmat, output_rotmatT, sizeof(output_rotmatT));
    }

    // Check the rotation matrix is valid
    if (!mat3_is_rotation(output_rotmat, MATRIX_IDENTITY_TOLERANCE)) {
        LOG("ERROR", "Computed rotation matrix is not a pure rotation matrix");
        return ERROR;
    }

    return OK;
}

StatusCode rotmat_tirs_to_cirs(
    // Outputs
    double output_rotmat[3][3],
    // Inputs
    const double earth_rotation_angle,
    const bool transpose) {
    
    // Local variables
    double output_rotmatT[3][3];

    mat3_initialise(output_rotmat);

    // Call to SOFA library
    iauRz(earth_rotation_angle, output_rotmat);

    if (transpose) {
        mat3_transpose(output_rotmatT, output_rotmat);
        memcpy(output_rotmat, output_rotmatT, sizeof(output_rotmatT));
    }

    // Check the rotation matrix is valid
    if (!mat3_is_rotation(output_rotmat, MATRIX_IDENTITY_TOLERANCE)) {
        LOG("ERROR", "Computed rotation matrix is not a pure rotation matrix");
        return ERROR;
    }

    return OK;
}

StatusCode rotmat_cirs_to_gcrs(
    // Outputs
    double output_rotmat[3][3],
    // Inputs
    const double tt_mjd2000,
    const int c2i_method,
    const bool transpose) {

    // Local variables
    const double jd_part_1 = 2451545.0;
    double output_rotmatT[3][3];

    mat3_initialise(output_rotmat);

    // Call to SOFA library to compute the rotation matrix
    switch (c2i_method) {
        case 0:
            iauC2i00a(jd_part_1, tt_mjd2000, output_rotmat);
            break;
        case 1:
            iauC2i00b(jd_part_1, tt_mjd2000, output_rotmat);
            break;
        case 2:
            iauC2i06a(jd_part_1, tt_mjd2000, output_rotmat);
            break;
        default:
            LOG("ERROR", "Invalid C2I method: %d", c2i_method);
            return ERROR;
    }

    if (transpose) {
        mat3_transpose(output_rotmatT, output_rotmat);
        memcpy(output_rotmat, output_rotmatT, sizeof(output_rotmatT));
    }

    // Check the rotation matrix is valid
    if (!mat3_is_rotation(output_rotmat, MATRIX_IDENTITY_TOLERANCE)) {
        LOG("ERROR", "Computed rotation matrix is not a pure rotation matrix");
        return ERROR;
    }

    return OK;
}
