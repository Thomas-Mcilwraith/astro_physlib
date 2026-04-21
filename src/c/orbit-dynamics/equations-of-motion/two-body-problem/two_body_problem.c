/*
 * two_body_problem.c
 *
 * Author: Thomas McIlwraith
 * Date: 18/04/2026
 * url: /orbit-dynamics/equations-of-motion/two-body-problem/two_body_problem.c
 */

#include "two_body_problem.h"

/**
 * See two_body_problem.h for IO description.
 *
 * pos must be in an inertial reference frame.
 * The elements of pos must have compatible units with mu.
 * out_acc will have the same spatial units as pos.
 *   (ie pos[km] -> out_acc[km/s^2])
 */
StatusCode two_body_problem(double out_du[6],
        // Inputs
        const double u[6],
        const double t,
        void* params){

    // Local variables
    StatusCode status = OK;
    TwoBodyProblemParams* p = (TwoBodyProblemParams*) params;
    double r_norm, r_direction_vec[3];
    double mu_over_r_squared;
    double pos[3] = {u[0], u[1], u[2]};
    (void) t;

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
    func ode = two_body_problem;
    double mu = 0.0;
    double state0[6], state1[6];
    double t0 = 0.0;
    double tf = 5400.0;
    double t = t0;
    int n_steps = 1000;
    double dt;

    // Initialise the log
    init_log();

    dt = (tf - t0) / n_steps;

    // Since we know the n_steps, define the solutions
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

    LOG("INFO", "mu = %f", mu);
    LOG("INFO", "vel = (%f, %f, %f)", state0[3], state0[4], state0[5]);

    params.mu = mu;
    r0s[0] = state0[0];
    r1s[0] = state0[1];
    r2s[0] = state0[2];
    v0s[0] = state0[3];
    v1s[0] = state0[4];
    v2s[0] = state0[5];

    for (int i = 0; i < n_steps; i++){

        // Integrate using RK4 to generate the state vector at the next time
        status = runge_kutta_4(state1,
                state0,
                dt,
                t0,
                ode,
                6,
                &params);

        if (status != OK){
            LOG("ERROR", "Failed to Two Body problem");
            return status;
        }

        r0s[i+1] = state1[0];
        r1s[i+1] = state1[1];
        r2s[i+1] = state1[2];
        v0s[i+1] = state1[3];
        v1s[i+1] = state1[4];
        v2s[i+1] = state1[5];

        t += dt;
        *state0 = *state1;
    }

    // Create the Parameters Evolution objects and write the file
    ParameterEvolution ephemeris[6] = {
        {POS_X, KM, r0s, n_steps},
        {POS_Y, KM, r1s, n_steps},
        {POS_Z, KM, r2s, n_steps},
        {VEL_X, KMS, v0s, n_steps},
        {VEL_Y, KMS, v1s, n_steps},
        {VEL_Z, KMS, v2s, n_steps}
    };

    status = write_parameter_evolution_file("test.pef", ephemeris, 6);
    if (status != OK){
        LOG("ERROR", "Failed to write parameter evolution file");
        return status;
    }

    return OK;
}
