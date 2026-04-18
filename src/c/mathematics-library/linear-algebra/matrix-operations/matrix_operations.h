#ifndef MATRIX_OPERATIONS_H
#define MATRIX_OPERATIONS_H

// Standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Local libraries
#include "../../../utilities/logging/log/log.h"

// Variable and Macro definitions

// Function prototypes

/**
 * @brief 
 * Computes the norm of a 3D vector.
 *
 * @param v The vector.
 * @return The norm of the vector.
 */
double vec3_norm(const double v[3]);

/**
 * @brief 
 * Computes the dot product of two 3D vectors.
 *
 * @param v The first vector.
 * @param w The second vector.
 * @return The dot product of the two vectors.
 */
double vec3_dot(const double v[3], const double w[3]);

/**
 * @brief 
 * Computes the unit vector of a 3D vector.
 *
 * @param out_unit The output unit vector.
 * @param v The input vector.
 * @return 0 if the vector is not zero length, 1 otherwise.
 */
int vec3_unit(double out_unit[3], const double v[3]);

#endif
