
/*
 * runge_kutta_4.c
 *
 * Author: Thomas McIlwraith
 * Date: 18/04/2026
 * url: /mathematics-library/numerical-methods/runge-kutta-4/runge_kutta_4.c
 */

#include "runge_kutta_4.h"

/**
 * u0 and u1 must have length dims.
 * ode must be a function pointer to a function with the signature: 
 *     func(double *out_du, const double *u, const double t, void* params);
 * 
 * params is a pointer to a struct which contains all constants required to
 * evaluate the ODE.
 * The void* format is so that different ODEs can be evaluated with this method.
 * In each ODE, params should be immediately cast to the expected type for that
 * ODE.
*/
StatusCode runge_kutta_4(
        // Outputs
        double* out_u1, 
        // Inputs
        const double* u0,
        const double dt,
        const double t0,
        const func ode,
        const int n_dims,
        void* params) {

    // Local variables
    StatusCode status = OK;
    double k1[n_dims], k2[n_dims], k3[n_dims], k4[n_dims];
    double k_buf[n_dims], u_buf[n_dims];

    if (!out_u1 || !u0 || !ode || n_dims <= 0 || dt == 0.0){
        LOG("ERROR", "Invalid input(s) to runge_kutta_4");
        return ERROR;
    }

    // Compute coefficients
    // At every step, we compute terms k_buf=k*dt/2 then u_buf=u0+k_buf
    status = ode(k1, u0, t0, params);
    if (status != OK){
        LOG("ERROR", "Failed to compute k1 in RK4");
        return status;
    }

    // Compute k2
    vec_scale(n_dims, k_buf, k1, 0.5 * dt);
    vec_add(n_dims, u_buf, u0, k_buf);
    status = ode(k2, u_buf, t0 + 0.5 * dt, params);
    if (status != OK){
        LOG("ERROR", "Failed to compute k2 in RK4");
        return status;
    }

    // Compute k3
    vec_scale(n_dims, k_buf, k2, 0.5 * dt);
    vec_add(n_dims, u_buf, u0, k_buf);
    status = ode(k3, u_buf, t0 + 0.5 * dt, params);
    if (status != OK){
        LOG("ERROR", "Failed to compute k3 in RK4");
        return status;
    }

    // Compute k4
    vec_scale(n_dims, k_buf, k3, dt);
    vec_add(n_dims, u_buf, u0, k_buf);
    status = ode(k4, u_buf, t0 + dt, params);
    if (status != OK){
        LOG("ERROR", "Failed to compute k4 in RK4");
        return status;
    }

    // Compute u1
    for (int i = 0; i < n_dims; i++){
        out_u1[i] = 
            u0[i] + dt * (k1[i] + 2.0 * k2[i] + 2.0 * k3[i] + k4[i]) / 6.0;
    }

    return OK;
}
