#ifndef INTERPOLATION_H
#define INTERPOLATION_H

// Standard libraries
#include <stdlib.h>
#include <math.h>

// Local libraries
#include "utilities/logging/log/log.h"

// Variable and Macro definitions

// Function prototypes

/**
 * @brief 
 * Generates a linearly spaced array of doubles between start_value and
 * end_value with step_size between each value.
 *
 * @note The array must be freed by the caller!
 *
 * @note The last step is always truncated to end_value.
 *
 * @param series The array of doubles to be generated.
 * @param number_of_points The number of points in the array.
 * @param start_value The start value of the array.
 * @param end_value The end value of the array.
 * @param step_size The step size between each value.
 * @return OK if successful, ERROR otherwise.
 */
StatusCode generate_linearly_spaced_array(
    // Outputs
    double** array,
    int* number_of_points,
    // Inputs
    const double start_value,
    const double end_value,
    const double step_size);

#endif
