/*
 * two_body_problem.c
 *
 * Author: Thomas McIlwraith
 * Date: 18/04/2026
 */

#include "two_body_problem.h"

/**
 * pos/vel must be in an inertial reference frame.
 * Make sure the units of u[:2], u[2:] and params->mu are consistent.
 * 
 * IMPORTANT: params must always be a pointer to a TwoBodyProblemParams type
 *            see mathematics-library/numerical-methods/runge_kutta_4.c
 * IMPORTANT: The two body problem is time invariant. We pass it here to ensure
 *            compatibility with the RKn integrator.
 */
StatusCode compute_two_body_derivative(
        // Outputs
        double out_du[6],
        // Inputs
        const double u[6],
        const double t,
        const void* params) {

    // Local variables
    const TwoBodyProblemParams* p = (const TwoBodyProblemParams*) params;
    double r_norm, mu_over_r_cubed;
    (void) t;

    // Compute the normal of the position vector.
    r_norm = vec3_norm(u);
    if (r_norm <= TWO_BODY_PROBLEM_SMALL_R_THRESHOLD) {
        LOG("ERROR", "Zero denominator detected in twobody_acceleration()");
        return ERROR;
    }

    // Since mu*(1/r_norm^3) is constant for every element, we compute it once
    // and store it to avoid recomputing it.
    mu_over_r_cubed = p->mu / (r_norm * r_norm * r_norm);

    out_du[0] = u[3];
    out_du[1] = u[4];
    out_du[2] = u[5];
    out_du[3] = - mu_over_r_cubed * u[0];
    out_du[4] = - mu_over_r_cubed * u[1];
    out_du[5] = - mu_over_r_cubed * u[2];

    return OK;
}

// /**
//  * Simple ephemeris generator, evaluates two body acceleration along a
//  * fixed timeseries with RK4 integration method.
//  */
// StatusCode generate_twobody_ephemeris(
//         // Outputs
//         double (*out_state_array)[6],
//         // Inputs
//         const TwoBodyProblemParams *params,
//         const double initial_state[6],
//         const double *time_array,
//         const int n_points
//         ) {

//     // Local variables
//     StatusCode status = OK;
//     func ode = twobody_acceleration;
//     double stepsize;  // measured backward i.e. (s = t_n - t_n-1)

//     if (!out_state_array || !params || !initial_state || !time_array
//         || n_points <= 0) {
//         LOG("ERROR", "Invalid input(s) to generate_twobody_ephemeris");
//         return ERROR;
//     }

//     // The first solution is always the initial condition
//     memcpy(out_state_array[0], initial_state, 6 * sizeof(double));

//     LOG("INFO", "Propagating Arc");

//     for (int i = 1; i < n_points; i++) {
//         stepsize = time_array[i] - time_array[i-1];

//         // Integrate Two-Body ODE using Runge-Kutta 4th order method
//         status = runge_kutta_4(out_state_array[i], out_state_array[i-1],
//             stepsize, time_array[i-1], ode, 6,
//             (void *)params
//         );
//         if (status != OK){
//             LOG("ERROR", "Failed to resolve Two Body Problem acceleration");
//             return status;
//         }
//     }

//     LOG("INFO", "Arc Complete");

//     return OK;
// }

// /**
//  * Example use case of two_body_problem.c.
//  *
//  * Generation of a Ephemeris (trajectory) file for an object experiencing
//  * un-perturbed keplerian motion.
//  *
//  * 1.  Load Initial Conditions (state vector and timespan)
//  * 2.  Compute Acceleration
//  * 3.  Integrate using RK4 to generate the state vector at the next time
//  * 4.  Generate the Ephemeris file for this trajectory
//  */
// int main(int argc, char *argv[]) {

//     // Local variables
//     StatusCode status = OK;
//     TwoBodyProblemParams params;
//     double mu, tf, dt;
//     double t0 = 0.0;
//     int n_steps = 1000;
//     double time_array[n_steps];
//     double state_array[n_steps][6];
//     double r0s[n_steps], r1s[n_steps], r2s[n_steps];
//     double v0s[n_steps], v1s[n_steps], v2s[n_steps];

//     // Initialise the log
//     init_log();

//     // Parse CLI arguments
//     if (argc - 1 != TWO_BODY_PROBLEM_C_NARGS) {
//         LOG("ERROR", "Incorrect program arguments");
//         LOG("INFO", "Required Args: <mu> <r1> <r2> <r3> <v1> <v2> <v3> <tf>");
//         return ERROR;
//     }
//     mu = strtod(argv[1], NULL);
//     tf = strtod(argv[8], NULL);
//     state_array[0][0] = strtod(argv[2], NULL);
//     state_array[0][1] = strtod(argv[3], NULL);
//     state_array[0][2] = strtod(argv[4], NULL);
//     state_array[0][3] = strtod(argv[5], NULL);
//     state_array[0][4] = strtod(argv[6], NULL);
//     state_array[0][5] = strtod(argv[7], NULL);

//     LOG("INFO", "Loaded Args:");
//     LOG("INFO", "mu = %f", mu);
//     LOG("INFO", "tf = %f", tf);
//     LOG("INFO", "pos = (%f, %f, %f)",
//         state_array[0][0], state_array[0][1], state_array[0][2]);
//     LOG("INFO", "vel = (%f, %f, %f)",
//         state_array[0][3], state_array[0][4], state_array[0][5]);

//     // Load parameters
//     params.mu = mu;

//     // Determine the stepsize
//     dt = (tf - t0) / (n_steps - 1);
//     // Create a time array from t0 to tf in n_steps
//     for (int i = 0; i < n_steps; i++) {
//         time_array[i] = t0 + i*dt;
//     }

//     // Generate the trajectory by integrating Two-Body ODE with RK4
//     status = generate_twobody_ephemeris(state_array, &params, state_array[0],
//         time_array, n_steps
//     );
//     if (status != OK) {
//         LOG("ERROR", "Failed to Generate Two-Body Ephemeris");
//         return status;
//     }

//     // For the output param evolution, we must convert row-major state_array to
//     // column slices
//     for (int i=0;i<n_steps;i++) {
//         r0s[i] = state_array[i][0];
//         r1s[i] = state_array[i][1];
//         r2s[i] = state_array[i][2];
//         v0s[i] = state_array[i][3];
//         v1s[i] = state_array[i][4];
//         v2s[i] = state_array[i][5];
//     }

//     // Create the ParameterEvolution
//     const int n_param_evos = 7;
//     ParameterEvolution ephemeris[] = {
//         {TIME, SECONDS_RELATIVE, time_array, n_steps},
//         {POS_X, KM, r0s, n_steps},
//         {POS_Y, KM, r1s, n_steps},
//         {POS_Z, KM, r2s, n_steps},
//         {VEL_X, KMS, v0s, n_steps},
//         {VEL_Y, KMS, v1s, n_steps},
//         {VEL_Z, KMS, v2s, n_steps}
//     };

//     // Write the ParameterEvolution file
//     status = write_parameter_evolution_file("two_body_problem.pef", ephemeris,
//          n_param_evos, "Two-Body Ephemeris"
//     );
//     if (status != OK){
//         LOG("ERROR", "Failed to write parameter evolution file");
//         return status;
//     }
//     LOG("INFO", "Parameter Evolution File written: %s", "two_body_problem.pef");

//     return OK;
// }
