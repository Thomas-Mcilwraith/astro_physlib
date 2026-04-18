#ifndef MATRIX_OPERATIONS_H
#define MATRIX_OPERATIONS_H

// Standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Local libraries
#include "../../../utilities/logging/log/log.h"

// Variable and Macro definitions

// Function prototypes
double vec3_norm(const double v[3]);
double vec3_dot(const double v[3], const double w[3]);
int vec3_unit(double out_unit[3], const double v[3]);

#endif
