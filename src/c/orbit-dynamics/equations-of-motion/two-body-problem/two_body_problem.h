#ifndef TWO_BODY_PROBLEM_H
#define TWO_BODY_PROBLEM_H

// Standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Local libraries
#include "utilities/logging/log/log.h"
#include "mathematics-library/linear-algebra/matrix-operations/matrix_operations.h"

// Variable and Macro definitions
#define TWO_BODY_PROBLEM_C_NARGS 7

// Function prototypes

/**
 * @brief 
 * Computes the acceleration of a satellite orbiting a central mass assuming
 * that both masses are point masses and m1 << m2, where m1 is the mass of the
 * satetlite and m2 is the mass of the central body.
 *
 * @param out_acc The inertial acceleration.
 * @param mu The planetary constant of the central body.
 * @param r The position of the secondary body.
 *
 * @return 0 if the calculation was successful, 1 otherwise.
 */
StatusCode two_body_problem(double out_acc[3],
        const double pos[3],
        const double mu
);

#endif
