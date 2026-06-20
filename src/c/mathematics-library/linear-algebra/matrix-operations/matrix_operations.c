/*
 * matrix_operations.c
 * 
 * Author: Thomas McIlwraith
 * Date: 18/04/2026
 * url: /mathematics-library/linear-algebra/matrix-operations/matrix_operations.c
 */

#include "matrix_operations.h"

double vec3_norm(const double v[3]){
    return sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}

/**
 * Make sure that out, v1 and v2 are all of length dims.
 */
StatusCode vec_add(const int dims,
        // Outputs
        double* out_sum,
        // Inputs
        const double* v1,
        const double* v2) {

    if (!out_sum || !v1 || !v2 || dims <= 0) {
        LOG("ERROR", "Invalid inputs");
        return ERROR;
    }

    for (int i = 0; i < dims; i++) {
        out_sum[i] = v1[i] + v2[i];
    }

    return OK;
}

StatusCode vec_scale(const int dims,
        // Outputs
        double* out_scaled,
        // Inputs
        const double* v,
        const double scale) {

    if (!v || dims <= 0) {
        LOG("ERROR", "Invalid inputs");
        return ERROR;
    }

    for (int i = 0; i < dims; i++) {
        out_scaled[i] = v[i] * scale;
    }

    return OK;
}

double vec3_dot(const double v[3], const double w[3]){
    return v[0]*w[0] + v[1]*w[1] + v[2]*w[2];
}

StatusCode vec3_unit(double out_unit[3],
        // Inputs
        const double v[3]){

    // Local variables
    const double norm = vec3_norm(v);

    if (norm < MATRIX_SMALL_NUMBER) {
        LOG("ERROR", "Tried to compute direction of 0 length vector");
        return ERROR;
    }

    out_unit[0] = v[0] / norm;
    out_unit[1] = v[1] / norm;
    out_unit[2] = v[2] / norm;

    return OK;
}

StatusCode mat3_mul(
        // Outputs
        double out[3][3],
        // Inputs
        const double m1[3][3],
        const double m2[3][3]) {

    // Local variables
    int i, j, k;

    if (!out || !m1 || !m2) {
        LOG("ERROR", "Invalid input (NULL pointer)");
        return ERROR;
    }
    if (out == m1 || out == m2) {
        LOG("ERROR", "Invalid input (output is input)");
        return ERROR;
    }

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            out[i][j] = 0.0;

            for (k = 0; k < 3; k++) {
                out[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }

    return OK;
}

StatusCode vec3_rotate(
        // Outputs
        double out[3],
        // Inputs
        const double mat[3][3],
        const double vec[3]) {

    if (!out || !mat || !vec) {
        LOG("ERROR", "Invalid input (NULL pointer)");
        return ERROR;
    }
    if (out == vec) {
        LOG("ERROR", "Invalid input (output is input)");
        return ERROR;
    }

    out[0] = mat[0][0] * vec[0] + mat[0][1] * vec[1] + mat[0][2] * vec[2];
    out[1] = mat[1][0] * vec[0] + mat[1][1] * vec[1] + mat[1][2] * vec[2];
    out[2] = mat[2][0] * vec[0] + mat[2][1] * vec[1] + mat[2][2] * vec[2];

    return OK;
}

void mat3_rotate_x(
        // Outputs
        double out[3][3],
        // Inputs
        const double alpha) {

    // Local variables
    const double c_alpha = cos(alpha);
    const double s_alpha = sin(alpha);

    out[0][0] = 1.0;
    out[0][1] = 0.0;
    out[0][2] = 0.0;

    out[1][0] = 0.0;
    out[1][1] = c_alpha;
    out[1][2] = s_alpha;

    out[2][0] = 0.0;
    out[2][1] = -s_alpha;
    out[2][2] = c_alpha;

    return;
}

void mat3_rotate_y(
        // Outputs
        double out[3][3],
        // Inputs
        const double alpha) {

    // Local variables
    const double c_alpha = cos(alpha);
    const double s_alpha = sin(alpha);

    out[0][0] = c_alpha;
    out[0][1] = 0.0;
    out[0][2] = -s_alpha;

    out[1][0] = 0.0;
    out[1][1] = 1.0;
    out[1][2] = 0.0;

    out[2][0] = s_alpha;
    out[2][1] = 0.0;
    out[2][2] = c_alpha;

    return;
}

void mat3_rotate_z(
        // Outputs
        double out[3][3],
        // Inputs
        const double alpha) {

    // Local variables
    const double c_alpha = cos(alpha);
    const double s_alpha = sin(alpha);

    out[0][0] = c_alpha;
    out[0][1] = s_alpha;
    out[0][2] = 0.0;

    out[1][0] = -s_alpha;
    out[1][1] = c_alpha;
    out[1][2] = 0.0;

    out[2][0] = 0.0;
    out[2][1] = 0.0;
    out[2][2] = 1.0;

    return;
}

StatusCode vec3_cross(
        // Outputs
        double out[3],
        // Inputs
        const double v1[3],
        const double v2[3]) {

    if (!out || !v1 || !v2) {
        LOG("ERROR", "Invalid input pointers");
        return ERROR;
    }

    const double x = v1[1] * v2[2] - v1[2] * v2[1];
    const double y = v1[2] * v2[0] - v1[0] * v2[2];
    const double z = v1[0] * v2[1] - v1[1] * v2[0];

    out[0] = x;
    out[1] = y;
    out[2] = z;

    return OK;
}
