/*
 * two_body_problem.c
 *
 * Author: Thomas McIlwraith
 * Date: 18/04/2026
 * url: /orbit-dynamics/equations-of-motion/two-body-problem/two_body_problem.c
 */

#include "two_body_problem.h"

/**
 * pos must be in an inertial reference frame.
 * The elements of pos must have compatible units with mu.
 * out_acc will have the same spatial units as pos and temporal units as mu.
 *   (ie pos[km] and mu[km^3/s^-2]-> out_acc[km/s^2])
 * 
 * IMPORTANT: params must always be a pointer to a TwoBodyProblemParams type
 *            see mathematics-library/numerical-methods/runge_kutta_4.c
 */
StatusCode twobody_acceleration(
        // Outputs
        double out_du[6],
        // Inputs
        const double u[6],
        const double t,
        void* params) {

    // Local variables
    StatusCode status = OK;
    TwoBodyProblemParams* p = (TwoBodyProblemParams*) params;
    double r_norm, r_direction_vec[3];
    double mu_over_r_squared;
    double pos[3] = {u[0], u[1], u[2]};
    (void) t;  // The two body problem is time invariant. We pass it here simply
               // to ensure compatibility with the integrator, it is not needed.

    r_norm = vec3_norm(pos);
    status = vec3_unit(r_direction_vec, pos);
    if (status != OK){
        LOG("ERROR", "Failed to compute direction of position vector");
        return status;
    }

    // Since mu*(1/r_norm^2) is constant for every element, we compute it once
    // and store it to avoid recomputing it
    mu_over_r_squared = p->mu / (r_norm * r_norm);

    out_du[0] = u[3];
    out_du[1] = u[4];
    out_du[2] = u[5];
    out_du[3] = - mu_over_r_squared * r_direction_vec[0];
    out_du[4] = - mu_over_r_squared * r_direction_vec[1];
    out_du[5] = - mu_over_r_squared * r_direction_vec[2];

    return status;
}

/**
 * Simple ephemeris generator, evaluates two body acceleration along a
 * timeseries with RK4 integration method.
 */
StatusCode generate_twobody_ephemeris(
        // Outputs
        double (*out_stateseries)[6],
        // Inputs
        const TwoBodyProblemParams *params,
        const double initial_state[6],
        const double *timeseries,
        const int n_points
        ) {

    // Local variables
    StatusCode status = OK;
    func ode = twobody_acceleration;
    double stepsize;  // measured backward i.e. (s = t_n - t_n-1)

    if (!out_stateseries || !params || !initial_state || !timeseries
        || n_points <= 0) {
        LOG("ERROR", "Invalid input(s) to generate_twobody_ephemeris");
        return ERROR;
    }

    // The first solution is always the initial condition
    memcpy(out_stateseries[0], initial_state, 6 * sizeof(double));

    for (int i = 1; i < n_points; i++) {
        stepsize = timeseries[i] - timeseries[i-1];

        status = runge_kutta_4(
            out_stateseries[i],
            out_stateseries[i-1],
            stepsize,
            timeseries[i-1],
            ode,
            6,
            params
        );

        if (status != OK){
            LOG("ERROR", "Failed to resolve Two Body Problem acceleration");
            return status;
        }
    }

    return OK;
}

/**
 * Example use case of two_body_problem.c.
 *
 * Generation of a Ephemeris (trajectory) file for an object experiencing
 * un-perturbed keplerian motion.
 *
 * 1.  Load Initial Conditions (state vector and timespan)
 * 2.  Compute Acceleration
 * 2a.   Integrate using RK4 to generate the state vector at the next time
 * 2b.   If t < tf, repeat from step 2a
 * 2c.   If t >= tf, exit
 * 3.  Generate the Ephemeris file for this trajectory
 */
int main(int argc, char *argv[]) {

    // Local variables
    StatusCode status = OK;
    TwoBodyProblemParams params;
    func ode = twobody_acceleration;
    double mu = 0.0;
    double state0[6], state1[6];
    double t0 = 0.0;
    double tf = 5400.0;
    double t = t0;
    int n_steps = 1000;
    double dt = (tf - t0) / (n_steps - 1);

    // Initialise the log
    init_log();

    // Since we know the n_steps, define the solutions
    double t_out[n_steps];
    double r0s[n_steps], r1s[n_steps], r2s[n_steps];
    double v0s[n_steps], v1s[n_steps], v2s[n_steps];

    // Parse CLI arguments
    if (argc - 1 != TWO_BODY_PROBLEM_C_NARGS) {
        status = ERROR;
        LOG("ERROR", "Incorrect program arguments");
        LOG("INFO", "Required Args: <mu> <r1> <r2> <r3> <v1> <v2> <v3>");
        return status;
    }
    mu = strtod(argv[1], NULL);
    state0[0] = strtod(argv[2], NULL);
    state0[1] = strtod(argv[3], NULL);
    state0[2] = strtod(argv[4], NULL);
    state0[3] = strtod(argv[5], NULL);
    state0[4] = strtod(argv[6], NULL);
    state0[5] = strtod(argv[7], NULL);

    LOG("INFO", "Loaded Args:");
    LOG("INFO", "mu = %f", mu);
    LOG("INFO", "pos = (%f, %f, %f)", state0[0], state0[1], state0[2]);
    LOG("INFO", "vel = (%f, %f, %f)", state0[3], state0[4], state0[5]);

    // save initial state in solution array
    params.mu = mu;
    t_out[0] = t;
    r0s[0] = state0[0];
    r1s[0] = state0[1];
    r2s[0] = state0[2];
    v0s[0] = state0[3];
    v1s[0] = state0[4];
    v2s[0] = state0[5];

    LOG("INFO", "Propagating Arc");
    for (int i = 1; i < n_steps; i++){

        // Integrate using RK4 to generate the state vector at the next time
        status = runge_kutta_4(state1, state0, dt, t, ode, 6, &params);
        if (status != OK){
            LOG("ERROR", "Failed to resolve Two Body Problem acceleration");
            return status;
        }

        // Save the results in the solution array
        t += dt;
        t_out[i] = t;
        r0s[i] = state1[0];
        r1s[i] = state1[1];
        r2s[i] = state1[2];
        v0s[i] = state1[3];
        v1s[i] = state1[4];
        v2s[i] = state1[5];

        // Update the state
        memcpy(state0, state1, 6 * sizeof(double));
    }
    LOG("INFO", "Arc Complete");

    // Create the Parameters Evolution objects and write the file
    ParameterEvolution ephemeris[7] = {
        {TIME, SECONDS_RELATIVE, t_out, n_steps},
        {POS_X, KM, r0s, n_steps},
        {POS_Y, KM, r1s, n_steps},
        {POS_Z, KM, r2s, n_steps},
        {VEL_X, KMS, v0s, n_steps},
        {VEL_Y, KMS, v1s, n_steps},
        {VEL_Z, KMS, v2s, n_steps}
    };

    status = write_parameter_evolution_file("test.pef", ephemeris, 7);
    if (status != OK){
        LOG("ERROR", "Failed to write parameter evolution file");
        return status;
    }
    LOG("INFO", "Parameter Evolution File written: %s", "test.pef");

    return OK;
}
