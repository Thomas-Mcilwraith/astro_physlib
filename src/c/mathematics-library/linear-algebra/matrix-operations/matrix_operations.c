/*
 * matrix_operations.c
 * 
 * Author: Thomas McIlwraith
 * url: /mathematics-library/linear-algebra/matrix-operations/matrix_operations.c
 *
 * Description:
 *   This file contains...
 */

#include "matrix_operations.h"

double vec3_norm(const double v[3]){
    return sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}

double vec3_dot(const double v[3], const double w[3]){
    return v[0]*w[0] + v[1]*w[1] + v[2]*w[2];
}

int vec3_unit(double out_unit[3],
        const double v[3]){

    // Local variables
    double norm = vec3_norm(v);

    if (norm == 0.0) {
        LOG("ERROR", "Tried to compute direction of 0 length vector");
        return 1;
    }

    out_unit[0] = v[0] / norm;
    out_unit[1] = v[1] / norm;
    out_unit[2] = v[2] / norm;
    return 0;
}


