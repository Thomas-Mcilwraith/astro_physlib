#ifndef EARTH_FIXED_FRAMES_H
#define EARTH_FIXED_FRAMES_H

// Standard libraries

// Local libraries
#include "../../../utilities/logging/log/log.h"
#include "../../../mathematics-library/linear-algebra/matrix-operations/matrix_operations.h"
#include "../../../utilities/constants/constants.h"

// Variable and Macro definitions
#define GEODETIC_LATTITUDE_TOLERANCE 1e-6
#define GEODETIC_LATTITUDE_MAX_ITER 20

// Function prototypes

/**
 * @brief
 * Computes the rotation matrix from GCRF to SEZ.
 *
 * @param output_rotmat The output rotation matrix.
 * @param r_site_gcrf The position of the site in GCRF.
 * @return 0 if the rotation matrix was computed successfully, 1 otherwise.
 */
StatusCode rotmat_gcrf_to_sez(
        // Outputs
        double output_rotmat[3][3],
        // Inputs
        const double r_site_gcrf[3]);

#endif

/**
 * @brief
 * Computes the position in ECEF from geodetic coordinates.
 *
 * @note All angles in radians. All distances in kilometers.
 *
 * @param r_site_gcrf The position of the site in ECEF.
 * @param geodetic_lat The geodetic latitude of the site.
 * @param longitude The longitude of the site.
 * @param height_above_ellipsoid The height above the earth surface ellipsoid.
 * @return 0 if the position was computed successfully, 1 otherwise.
 */
StatusCode latlon_to_ecef(
        // Outputs
        double r_ecef[3],
        // Inputs
        const double geodetic_lat,
        const double longitude,
        const double height_above_ellipsoid
        );

/**
 * @brief
 * Computes the position in geodetic coordinates from ECEF.
 *
 * @note All angles in radians. All distances in kilometers.
 *
 * @param r_ecef The position in ECEF.
 * @param geodetic_lat The geodetic latitude of the site.
 * @param longitude The longitude of the site.
 * @param height_above_ellipsoid The height above the earth surface ellipsoid.
 * @return 0 if the position was computed successfully, 1 otherwise.
 */
StatusCode ecef_to_latlon(
        // Outputs
        double *geodetic_lat,
        double *longitude,
        double *height_above_ellipsoid,
        // Inputs
        const double r_ecef[3]);

/**
 * @brief
 * Converts geocentric lattitude to geodetic.
 *
 * @note All angles in radians.
 *
 * @param geocentric_lat Geocentric lattitude
 * @param e_ellip Eccentricity of the earth-surface ellipsiod. See WGS-84.
 */
double geocentric_to_geodetic_lat(double geocentric_lat);

/**
 * @brief
 * Computes the rotation matrix from ITRF to TIRF
 *
 * @note
 * All angles are in radians.
 *
 * @param output_rotmat The output rotation matrix.
 * @param x_polar_motion_angle The x polar motion angle.
 * @param y_polar_motion_angle The y polar motion angle.
 * @return 0 if the rotation matrix was computed successfully, 1 otherwise.
 */
StatusCode rotmat_itrs_to_tirs(
        // Outputs
        double output_rotmat[3][3],
        // Inputs
        const double x_polar_motion_angle,
        const double y_polar_motion_angle);

/**
 * @brief
 * Calculates the earth rotation angle
 * 
 * @param mjd2000_ut1 Modified Julian Date 2000 @ UT1
 * @return The earth rotation angle
 */
double earth_rotation_angle(const double mjd2000_ut1);

/**
 * @brief
 * Calculate the rotation matrix from TIRS to CIRS. Such that M x v_tirs = v_cirs
 * 
 * @note
 * Earth rotation angle in radians.
 * 
 * @param output_rotmat The output rotation matrix
 * @param earth_rotation_angle Earth rotation angle, derived directly from UT1.
 */
StatusCode rotmat_tirs_to_cirs(
    // Outputs
    double output_rotmat[3][3],
    // Inputs
    double earth_rotation_angle);