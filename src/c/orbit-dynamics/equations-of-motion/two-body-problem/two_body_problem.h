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
int two_body_problem(double out_acc[3],
        const double *mu,
        const double r[3]);

#endif
