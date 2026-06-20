/*
 * orbital_frames.c
 *
 * Author: Thomas McIlwraith
 * Date: 18/04/2026
 */

#include "orbital_frames.h"
#include "../../../utilities/logging/log/log.h"
#include "../../../mathematics-library/linear-algebra/matrix-operations/matrix_operations.h"

/**
 * @note
 * GCRF: ECI frame. LVLH: RTN frame
 * */
StatusCode rotmat_gcrf_to_lvlh(
        //Outputs
        double rotmat_out[3][3],
        //Inputs
        const double r_gcrf[3],
        const double v_gcrf[3]) {

    // Local variables
    StatusCode status = OK;
    double r_norm, v_norm, r_unit[3], v_unit[3];
    double w[3], w_norm, w_unit[3];

    r_norm = vec3_norm(r_gcrf);
    v_norm = vec3_norm(v_gcrf);

    if (r_norm < MATRIX_SMALL_NUMBER || v_norm < MATRIX_SMALL_NUMBER) {
        LOG("ERROR", "Tried to convert GCRF to LVLH with 0 length vector");
        return ERROR;
    }

    // Compute the unit vectors
    vec3_unit(r_unit, r_gcrf);
    vec3_unit(v_unit, v_gcrf);

    // Compute the third vector
    status = vec3_cross(w, r_unit, v_unit);
    status = vec3_unit(w_unit, w);
    if (status != OK) {
        LOG("ERROR", "Failed to compute w unit vector");
        return status;
    }

    w_norm = vec3_norm(w);
    if (w_norm < ORBITAL_ANG_MOMENTUM_THRESHOLD) {
        LOG("ERROR", "r_gcrf and v_gcrf are ~parallel");
        return ERROR;
    }

    // Recompute the v vector, to ensure orthogonality for non-cirular orbits
    // NOTE: The v vec is *in the direction* of the v vector, not aligned to it
    status = vec3_cross(v_unit, w_unit, r_unit);
    
    // Assemble the DCM
    rotmat_out[0][0] = r_unit[0];
    rotmat_out[0][1] = r_unit[1];
    rotmat_out[0][2] = r_unit[2];

    rotmat_out[1][0] = v_unit[0];
    rotmat_out[1][1] = v_unit[1];
    rotmat_out[1][2] = v_unit[2];

    rotmat_out[2][0] = w_unit[0];
    rotmat_out[2][1] = w_unit[1];
    rotmat_out[2][2] = w_unit[2];

    return OK;
}

