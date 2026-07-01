#ifndef MATRIX_OPERATIONS_H
#define MATRIX_OPERATIONS_H

// Standard libraries
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

// Local libraries
#include "utilities/logging/log/log.h"

// Variable and Macro definitions
#define MATRIX_SMALL_NUMBER 1e-13
#define MATRIX_IDENTITY_TOLERANCE 1e-13
#define MATRIX_DET_TOLERANCE 1e-13

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
 * Computes the sum of two nD vectors.
 *
 * @param dims The number of dimensions of the vectors.
 * @param v1 The first vector.
 * @param v2 The second vector.
 */
StatusCode vec_add(const int dims,
        double* out_sum,
        const double* v1,
        const double* v2);

/**
 * @brief 
 * Scales a vector by a scalar.
 *
 * @param dims The number of dimensions of the vector.
 * @param out_scaled The scaled vector.
 * @param v The vector to scale.
 * @param scale The scalar to scale by.
 */
StatusCode vec_scale(const int dims,
        double* out_scaled,
        const double* v,
        const double scale);

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
StatusCode vec3_unit(double out_unit[3], const double v[3]);

/**
 * @brief
 * Multiplies two 3x3 matrices.
 *
 * @param out The output matrix.
 * @param m1 The first matrix.
 * @param m2 The second matrix.
 * @return 0 if the multiplication was successful, 1 otherwise.
 */
StatusCode mat3_mul(
        // Outputs
        double out[3][3],
        // Inputs
        const double m1[3][3],
        const double m2[3][3]);

/**
 * @brief
 * Rotates a 3D vector by a 3x3 matrix.
 *
 * @param out The output vector.
 * @param mat The rotation matrix to apply.
 * @param vec The vector to rotate.
 * @return 0 if the rotation was successful, 1 otherwise.
 */
StatusCode vec3_rotate(
        // Outputs
        double out[3],
        // Inputs
        const double mat[3][3],
        const double vec[3]);


/**
 * @brief
 * Generates a rotation matrix corresponding to a rotation about the x-axis by
 * an angle alpha.
 *
 * @note
 * The convention is the **right-hand rule**. This represents an **active** rotation.
 *
 * @param out The output matrix.
 * @param alpha The angle to rotate by.
 */
void mat3_rotate_x(
        // Outputs
        double out[3][3],
        // Inputs
        const double alpha);

/**
 * @brief
 * Generates a rotation matrix corresponding to a rotation about the y-axis by
 * an angle alpha.
 *
 * @note
 * The convention is the **right-hand rule**. This represents an **active** rotation.
 *
 * @param out The output matrix.
 * @param alpha The angle to rotate by.
 */
void mat3_rotate_y(
        // Outputs
        double out[3][3],
        // Inputs
        const double alpha);

/**
 * @brief
 * Generates a rotation matrix corresponding to a rotation about the z-axis by
 * an angle alpha.
 *
 * @note
 * The convention is the **right-hand rule**. This represents an **active** rotation.
 *
 * @param out The output matrix.
 * @param alpha The angle to rotate by.
 */
void mat3_rotate_z(
        // Outputs
        double out[3][3],
        // Inputs
        const double alpha);

/**
 * @brief
 * Computes the cross product of two 3D vectors.
 *
 * @param out The output vector.
 * @param v1 The first vector.
 * @param v2 The second vector.
 * @return 0 if the cross product was successful, 1 otherwise.
 */
StatusCode vec3_cross(
        // Outputs
        double out[3],
        // Inputs
        const double v1[3],
        const double v2[3]);

/**
 * @brief
 * Computes the transpose of a 3x3 matrix.
 *
 * @param m_transposed The output transposed matrix.
 * @param m The input matrix.
 */
void mat3_transpose(
        // Outputs
        double m_transposed[3][3],
        // Inputs
        const double m[3][3]);

/**
 * @brief
 * Computes the determinant of a 3x3 matrix.
 *
 * @param m The matrix.
 * @return The determinant of the matrix.
 */
double mat3_det(const double m[3][3]);

/**
 * @brief
 * Checks if a 3x3 matrix is the identity matrix.
 *
 * @param m The matrix.
 * @param tolerance The tolerance to use for the comparison.
 * @return True if the matrix is the identity matrix, false otherwise.
 */
bool mat3_is_identity(const double m[3][3], const double tolerance);

/**
 * @brief
 * Checks if a 3x3 matrix is orthogonal.
 *
 * @param m The matrix.
 * @param tolerance The tolerance to use for the comparison.
 * @return True if the matrix is orthogonal, false otherwise.
 */
bool mat3_is_orthogonal(const double m[3][3], const double tolerance);

/**
 * @brief
 * Checks if a 3x3 matrix is a pure rotation matrix.
 *
 * @param m The matrix.
 * @param tolerance The tolerance to use for the comparison.
 * @return True if the matrix is a rotation matrix, false otherwise.
 */
bool mat3_is_rotation(const double m[3][3], const double tolerance);

#endif
