/*
 * two_body_problem.c
 *
 * Author: Thomas McIlwraith
 * url: /orbit-dynamics/equations-of-motion/two-body-problem/two_body_problem.c
 *
 * Description:
 *   This file contains...
 */

#include "two_body_problem.h"

int two_body_problem(double out_acc[3],
        // Inputs
        const double *mu,
        const double r[3]){

    // Local variables
    int status = 0;
    double r_norm, r_direction_vec[3];
    double intermediate_value;

    r_norm = vec3_norm(r);
    status = vec3_unit(r_direction_vec, r);
    if (status != 0){
        LOG("ERROR", "Failed to compute direction of r");
        return status;
    }

    // Since mu*(1/r_norm^2) is constant for every element, we compute it once
    // and store it to avoid recomputing it
    intermediate_value = - *mu / (r_norm * r_norm);

    out_acc[0] = r_direction_vec[0] * intermediate_value;
    out_acc[1] = r_direction_vec[1] * intermediate_value;
    out_acc[2] = r_direction_vec[2] * intermediate_value;

    return status;
}

int main(int argc, char *argv[]) {

    // Local variables
    int status = 0;
    double mu = 0.0;
    double pos[3], vel[3], acc[3];

    // Initialise the log.
    init_log();

    if (argc - 1 != TWO_BODY_PROBLEM_C_NARGS) {
        int status = 1;
        LOG("ERROR", "Incorrect program arguements");
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

    printf("mu = %f\n", mu);
    printf("r = (%f, %f, %f)\n", pos[0], pos[1], pos[2]);
    printf("v = (%f, %f, %f)\n", vel[0], vel[1], vel[2]);

    status = two_body_problem(acc, &mu, pos);
    if (status != 0){
        LOG("ERROR", "Failed to compute a");
        return status;
    }

    printf("a = (%f, %f, %f)\n", acc[0]*1000, acc[1]*1000, acc[2]*1000);

    return 0;
}
