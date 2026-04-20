#ifndef PARAMETER_EVOLUTION_FILE_H
#define PARAMETER_EVOLUTION_FILE_H

// Standard libraries
#include <stdarg.h>
#include <stdlib.h>

// Local libraries
#include "../../../utilities/logging/log/log.h"

// Variable and Macro definitions
#define CHARS_PER_WORD 22        // Formatting
#define WORD_PRECISION 8

#define ACC "acc"                // Accepted variable names in header
#define ACC_X "acc_x"            // MUST NEVER BE LONGER THAN 10 CHARS
#define ACC_Y "acc_y"
#define ACC_Z "acc_z"
#define ALT "alt"
#define RHO "rho_alt_x"
#define QUAT "quat"

#define KM "km"                  // Accepted units in header
#define M "m"                    // MUST NEVER BE LONGER THAN 10 CHARS
#define S "s"
#define MS "ms"
#define MS2 "ms^2"
#define KMS "km/s"
#define KMS2 "km/s^2"
#define KGM3 "kg/m^3"
#define NO_UNIT "-"

// Data structure definitions

/**
 * @brief 
 * This struct is an interface between a std C array and the parameter
 * evolution file (.pef). A list of these structs can be written to a .pef
 *
 * @param name The name of the parameter. (selected from the list above)
 * @param units The units of the parameter. (selected from the list above)
 * @param values pointer to the array of values.
 * @param n_values The number of values in the array.
 */
typedef struct {
    const char* name;
    const char* units;
    const double* values;
    const int n_values;
} ParameterEvolution;

// Function prototypes

#endif
