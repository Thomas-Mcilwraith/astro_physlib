#ifndef PARAMETER_EVOLUTION_FILE_H
#define PARAMETER_EVOLUTION_FILE_H

// Standard libraries
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

// Local libraries
#include "../../../utilities/logging/log/log.h"

// Variable and Macro definitions
#define CHARS_PER_WORD 10        // Formatting
#define CHARS_PER_COL 23
#define WORD_PRECISION 8
#define VAR_UNITS_SEPARATOR "~"

#define ACC "ACC"                // Accepted variable names in header
#define ACC_X "ACC_X"            // MUST NEVER BE LONGER THAN 10 CHARS
#define ACC_Y "ACC_Y"
#define ACC_Z "ACC_Z"
#define POS_X "POS_X"
#define POS_Y "POS_Y"
#define POS_Z "POS_Z"
#define VEL_X "VEL_X"
#define VEL_Y "VEL_Y"
#define VEL_Z "VEL_Z"
#define MASS "MASS"
#define ALT "ALT"
#define RHO "RHO"
#define QUAT "QUAT"

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
    char name[CHARS_PER_WORD];
    char units[CHARS_PER_WORD];
    double* values;
    int n_values;
} ParameterEvolution;

// Function prototypes

/**
 * @brief
 * Write an array of Parameter evolution arrays to a file.
 * 
 * @param filename The path of the file to write to.
 * @param params Array pointer to a list of parameter evolutions.
 * @param n_params length of params.
 */
StatusCode write_parameter_evolution_file(
        // Inputs
        const char* filename,
        const ParameterEvolution* params,
        const int n_params);

/**
 * @brief
 * Read an array of Parameter evolution arrays from a file.
 * 
 * @param out_params Array pointer to a list of parameter evolutions.
 * @param out_n_params length of out_params.
 * @param out_n_values length of the values in the ParameterEvolution.
 * @param filename The path of the file to read from.
 */
StatusCode read_parameter_evolution_file(ParameterEvolution** out_params, 
        int* out_n_params,
        int* out_n_values,
        const char* filename);

/**
 * @brief 
 * Helper function - Remove trailing whitespace from a string.
 *
 * @param s The string to remove trailing whitespace from.
 */
void rtrim(char* s);

#endif
