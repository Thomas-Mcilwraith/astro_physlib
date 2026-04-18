/*
 * matrix_operations.c
 * 
 * Author: Thomas McIlwraith
 * Date: 18/04/2026
 * url: /mathematics-library/linear-algebra/matrix-operations/matrix_operations.c
 */

#include "matrix_operations.h"

/**
 * See matrix_operations.h for IO description.
 */
double vec3_norm(const double v[3]){
    return sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}

/**
 * See matrix_operations.h for IO description.
 */
double vec3_dot(const double v[3], const double w[3]){
    return v[0]*w[0] + v[1]*w[1] + v[2]*w[2];
}

/**
 * See matrix_operations.h for IO description.
 *
 * Raise an error if input vector is of length 0.
 */
int vec3_unit(double out_unit[3],
        const double v[3]){

    // Local variables
    int status = 0;
    double norm = vec3_norm(v);

    if (norm == 0.0) {
        status = 1;
        LOG("ERROR", "Tried to compute direction of 0 length vector");
        return status;
    }

    out_unit[0] = v[0] / norm;
    out_unit[1] = v[1] / norm;
    out_unit[2] = v[2] / norm;

    return status;
}


