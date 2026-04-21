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
StatusCode two_body_problem(double out_acc[3],
        // Inputs
        const double pos[3],
        const double mu){

    // Local variables
    StatusCode status = OK;
    double r_norm, r_direction_vec[3];
    double mu_over_r_squared;

    r_norm = vec3_norm(pos);
    status = vec3_unit(r_direction_vec, pos);
    if (status != OK){
        LOG("ERROR", "Failed to compute direction of position vector");
        return status;
    }

    // Since mu*(1/r_norm^2) is constant for every element, we compute it once
    // and store it to avoid recomputing it
    mu_over_r_squared = mu / (r_norm * r_norm);

    out_acc[0] = - mu_over_r_squared * r_direction_vec[0];
    out_acc[1] = - mu_over_r_squared * r_direction_vec[1];
    out_acc[2] = - mu_over_r_squared * r_direction_vec[2];

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
    double mu = 0.0;
    double pos[3], vel[3], acc[3];

    // Initialise the log
    init_log();

    // Parse CLI arguments
    if (argc - 1 != TWO_BODY_PROBLEM_C_NARGS) {
        status = ERROR;
        LOG("ERROR", "Incorrect program arguments");
        LOG("INFO", "Required Args: <mu> <r1> <r2> <r3> <v1> <v2> <v3>");
        return status;
    }
    mu = strtod(argv[1], NULL);
    pos[0] = strtod(argv[2], NULL);
    pos[1] = strtod(argv[3], NULL);
    pos[2] = strtod(argv[4], NULL);
    vel[0] = strtod(argv[5], NULL);
    vel[1] = strtod(argv[6], NULL);
    vel[2] = strtod(argv[7], NULL);

    LOG("INFO", "mu = %f", mu);
    LOG("INFO", "pos = (%f, %f, %f)", pos[0], pos[1], pos[2]);
    LOG("INFO", "vel = (%f, %f, %f)", vel[0], vel[1], vel[2]);

    status = two_body_problem(acc, pos, mu);
    if (status != 0){
        LOG("ERROR", "Failed to compute Two Body acceleration");
        return status;
    }

    LOG("INFO", "acc = (%f, %f, %f)", acc[0], acc[1], acc[2]);

    return OK;
}
