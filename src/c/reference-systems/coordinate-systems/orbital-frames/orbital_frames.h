#ifndef ORBITAL_FRAMES_H
#define ORBITAL_FRAMES_H

// Standard libraries

// Local libraries
#include "../../../utilities/logging/log/log.h"
#include "../../../mathematics-library/linear-algebra/matrix-operations/matrix_operations.h"

// Variable and Macro definitions
#define ORBITAL_ANG_MOMENTUM_THRESHOLD 1e-6

// Function prototypes

/**
 * @brief
 * Converts a GCRF frame to a LVLH frame.
 *
 * @param rotmat_out (*double[3][3]) The output rotation matrix.
 * @param r_gcrf (*double[3]) The position vector in GCRF.
 * @param v_gcrf (*double[3]) The velocity vector in GCRF.
 * @return 0 if the conversion was successful, 1 otherwise.
 */
StatusCode rotmat_gcrf_to_lvlh(
        //Outputs
        double rotmat_out[3][3],
        //Inputs
        const double r_gcrf[3],
        const double v_gcrf[3]);

#endif
