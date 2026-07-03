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
 * @brief Generate the rotation matrix from ITRS to TIRS
 *
 * @note All angles in radians. 
 *
 * @param output_rotmat The rotation matrix from ITRS to TIRS
 * @param xp The X-coordinate of the Celestial Intermediate Pole (in radians)
 * @param yp The Y-coordinate of the Celestial Intermediate Pole (in radians)
 * @param tio_locator The TIO locator (in radians)
 * @param transpose Transpose the output matrix
 * @return StatusCode
 */
StatusCode rotmat_itrs_to_tirs(
    // Outputs
    double output_rotmat[3][3],
    // Inputs
    const double xp,
    const double yp,
    const double tio_locator,
    const bool transpose);

/**
 * @brief Generate the rotation matrix from TIRS to CIRS
 *
 * @note All angles in radians
 *
 * @param output_rotmat TIRS to CIRS rotation matrix
 * @param earth_rotation_angle The Earth Rotation Angle, in radians.
 * @param transpose Transpose the output matrix
 */
StatusCode rotmat_tirs_to_cirs(
    // Outputs
    double output_rotmat[3][3],
    // Inputs
    const double earth_rotation_angle,
    const bool transpose);

/**
 * @brief Generate the rotation matrix from CIRS to GCRS
 * 
 * @note CIP offsets are included in IERS/Celestrak EOP data.
 *
 * @param output_rotmat CIRS to GCRS rotation matrix
 * @param tt_mjd2000 The Terrestrial Time in Modified Julian Date (MJD) 2000
 * @param c2i_method The C2I method to use:
 *      0: IAU 2000A
 *      1: IAU 2000B
 *      2: IAU 2006A
 * @param dX_cip CIP offset in radians. Only required if c2i_method==2.
 * @param dX_cip CIP offset in radians. Only required if c2i_method==2.
 * @param transpose Transpose the output matrix
 */
StatusCode rotmat_cirs_to_gcrs(
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
