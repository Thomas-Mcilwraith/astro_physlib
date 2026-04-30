#ifndef TWO_BODY_PROBLEM_H
#define TWO_BODY_PROBLEM_H

// Standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Local libraries
#include "utilities/logging/log/log.h"
#include "mathematics-library/linear-algebra/matrix-operations/matrix_operations.h"
#include "mathematics-library/numerical-methods/runge-kutta-4/runge_kutta_4.h"
#include "file-io/internal-products/parameter-evolution-file/parameter_evolution_file.h"

// Variable and Macro definitions
#define TWO_BODY_PROBLEM_C_NARGS 7

typedef struct {
    double mu;
} TwoBodyProblemParams;

// Function prototypes

/**
 * @brief 
 * Computes the acceleration of a satellite orbiting a central mass assuming
 * that both masses are point masses and m1 << m2, where m1 is the mass of the
 * satetlite and m2 is the mass of the central body.
 *
 * @param out_du The derivative of the state vector.
 * @param u The state vector.
 * @param t The time.
 * @param params The parameters of the problem.
 */
StatusCode twobody_acceleration(
        // Outputs
        double out_du[6],
        // Inputs
        const double u[6],
        const double t,
        void* params);

#endif
