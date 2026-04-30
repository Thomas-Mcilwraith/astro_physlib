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
        LOG("ERROR", "Invalid input to vec_add");
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
        LOG("ERROR", "Invalid input to vec_scale");
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
    double norm = vec3_norm(v);

    if (norm == 0.0) {
        LOG("ERROR", "Tried to compute direction of 0 length vector");
        return ERROR;
    }

    out_unit[0] = v[0] / norm;
    out_unit[1] = v[1] / norm;
    out_unit[2] = v[2] / norm;

    return OK;
}


