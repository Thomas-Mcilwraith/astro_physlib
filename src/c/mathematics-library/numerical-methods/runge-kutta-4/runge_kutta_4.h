#ifndef RUNGE_KUTTA_4_H
#define RUNGE_KUTTA_4_H

// Standard libraries

// Local libraries
#include "utilities/logging/log/log.h"
#include "mathematics-library/linear-algebra/matrix-operations/matrix_operations.h"

// Variable and Macro definitions

/**
 * @brief
 * Create a function pointer to an ODE. ALL ODE functions must have the same
 * signature.
 * 
 * @param out_du The derivative of the ODE. (variable output)
 * @param u The current state. (constant input)
 * @param t The current time. (constant input)
 * @param params Any parameters required to resolve the ODE which are not
 *               contained in u must be passed in a struct. The null pointer
 *               is a placeholder for a struct of unknown shape.
 */
typedef StatusCode (*func)(
        double* out_du,
        const double* u,
        const double t,
        void* params);

// Function prototypes

/**
 * @brief
 * Runge-Kutta 4 method for solving ODEs.
 *
 * @param out_u1 The state at the next time step. (t0+out_dt)
 * @param out_dt The time step taken by the solution.
 * @param u0 The initial state.
 * @param t0 The initial time.
 * @param ode The ODE to solve.
 * @param n_dims The number of dimensions of the ODE.
 * @param params Any parameters required to resolve the ODE
 */
StatusCode runge_kutta_4(
        double* out_u1,
        const double* u0,
        const double dt,
        const double t0,
        const func ode,
        const int n_dims,
        void* params);

#endif
