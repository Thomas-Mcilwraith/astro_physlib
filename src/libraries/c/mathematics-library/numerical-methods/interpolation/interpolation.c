/*
 * interpolation.c
 * 
 * Author: Thomas McIlwraith
 * Date: 18/04/2026
 *
 */

#include "interpolation.h"

StatusCode generate_linearly_spaced_array(
    // Outputs
    double** array,
    int* number_of_points,
    // Inputs
    const double start_value,
    const double end_value,
    const double step_size) {

    if (array == NULL || number_of_points == NULL) {
        LOG(ERROR, "Bad pointers to generate_linearly_spaced_array");
        return ERROR;
    }

    if (start_value >= end_value) {
        LOG(ERROR, "Start value must be less than end value");
        return ERROR;
    }

    // Calculate the number of points and allocate the array
    *number_of_points = (int)ceil(((end_value - start_value)/step_size + 1));
    *array = malloc(*number_of_points * sizeof(double));
    if (*array == NULL) {
        LOG(ERROR, "Failed to allocate memory for linearly spaced array.");
        return ERROR;
    }

    // Fill the array
    for (int i = 0; i < *number_of_points; i++) {
        if (i == *number_of_points - 1) {
            (*array)[i] = end_value;
        } else {
            (*array)[i] = start_value + (i * step_size);
        }
    }

    return OK;
}
