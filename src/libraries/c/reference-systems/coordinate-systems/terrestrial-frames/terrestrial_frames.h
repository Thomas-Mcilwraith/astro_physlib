#ifndef TERRESTRIAL_FRAMES_H
#define TERRESTRIAL_FRAMES_H

// Standard libraries
#include <stdbool.h>

// Local libraries
#include "utilities/logging/log/log.h"
#include "mathematics-library/linear-algebra/matrix-operations/matrix_operations.h"
#include "external/sofa/sofa.h"

// Variable and Macro definitions

// Function prototypes

/**
 * @brief Generate the rotation matrix from TIRS -> ITRS.
 *
 * @note All angles in radians. 
 * @note Such that r_itrs = output_rotmat x v_tirs
 *
 * @param output_rotmat The rotation matrix from TIRS -> ITRS
 * @param xp The X-coordinate of the Celestial Intermediate Pole (in radians)
 * @param yp The Y-coordinate of the Celestial Intermediate Pole (in radians)
 * @param tio_locator The TIO locator (in radians)
 * @param transpose Transpose the output matrix
 */
StatusCode rotmat_tirs_to_itrs(
    // Outputs
    double output_rotmat[3][3],
    // Inputs
    const double xp,
    const double yp,
    const double tio_locator,
    const bool transpose);

/**
 * @brief Generate the rotation matrix from CIRS -> TIRS
 *
 * @note All angles in radians
 * @note Such that r_tirs = output_rotmat x r_cirs
 *
 * @param output_rotmat CIRS -> TIRS rotation matrix
 * @param earth_rotation_angle The Earth Rotation Angle, in radians.
 * @param transpose Transpose the output matrix
 */
StatusCode rotmat_cirs_to_tirs(
    // Outputs
    double output_rotmat[3][3],
    // Inputs
    const double earth_rotation_angle,
    const bool transpose);

/**
 * @brief Generate the rotation matrix from GCRS -> CIRS
 * 
 * @note CIP offsets are included in IERS/Celestrak EOP data.
 * @note Such that r_cirs = output_rotmat x r_gcrs
 *
 * @param output_rotmat GCRS -> CIRS rotation matrix
 * @param tt_mjd2000 The Terrestrial Time in Modified Julian Date (MJD) 2000
 * @param c2i_method The C2I method to use:
 *      0: IAU 2000A
 *      1: IAU 2000B
 *      2: IAU 2006A
 * @param dX_cip CIP offset in radians. Only required if c2i_method==2.
 * @param dY_cip CIP offset in radians. Only required if c2i_method==2.
 * @param transpose Transpose the output matrix
 */
StatusCode rotmat_gcrs_to_cirs(
    // Outputs
    double output_rotmat[3][3],
    // Inputs
    const double tt_mjd2000,
    const int c2i_method,
    const double dX_cip,
    const double dY_cip,
    const bool transpose
    );

/**
 * @brief Compute the TIO locator for a given terrestrial time
 * 
 * @param tt_jd Terrestrial time, expressed in julian days.
 */
double get_tio_locator(const double tt_jd);

/**
 * @brief Compute Earth rotation angle
 * 
 * @param ut1_jd UT1 time, expressed in Julian days.
 */
double get_earth_rotation_angle(const double ut1_jd);
#endif
