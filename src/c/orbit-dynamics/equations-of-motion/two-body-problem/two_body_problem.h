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
#define TWO_BODY_PROBLEM_C_NARGS 8
#define TWO_BODY_PROBLEM_SMALL_R_THRESHOLD 1e-8

typedef struct {
    double mu;
} TwoBodyProblemParams;

// Function prototypes

/**
 * @brief
 * Computes the state vector derivative of a satellite orbiting a central mass
 * assuming that both masses are point masses and m1 << m2, where m1 is the
 * mass of the satetlite and m2 is the mass of the central body.
 *
 * @param out_du The derivative of the state vector.
 * @param u The state vector.
 * @param t The time, not required by this function directly, but is passed to
 *          ensure compatiblity with the integrators. Should be immediately
 *          cast to void.
 * @param params The TwoBodyProblemParams parameters of the problem. In this
 *               case, only mu.
 */
StatusCode compute_two_body_derivative(
        // Outputs
        double out_du[6],
        // Inputs
        const double u[6],
        const double t,
        const void* params);


// /**
//  * @brief
//  * Generates an ephemeris along a predefined, fixed timespan by integrating the
//  * Two-Body ODE using RK4.
//  * 
//  * @param out_state_array 6-by-n_steps array of state vectors.
//  * @param params The parameters of the problem. In this case, only mu.
//  * @param initial_state The initial condition of the ephemeris.
//  * @param time_array The fixed time array to evaluate the trajectory along. The
//  *                   first value must correspond to the time of inital_state.
//  * @param n_points Number of points in the time_array.
//  */
// StatusCode generate_twobody_ephemeris(
//         // Outputs
//         double (*out_state_array)[6],
//         // Inputs
//         const TwoBodyProblemParams *params,
//         const double initial_state[6],
//         const double *time_array,
//         const int n_points
//         );

#endif
