#ifndef PARAMETER_EVOLUTION_FILE_H
#define PARAMETER_EVOLUTION_FILE_H

// Standard libraries
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Local libraries
#include "utilities/logging/log/log.h"
#include "reference-systems/time-systems/time-formats/time_formats.h"

// Variable and Macro definitions
#define CHARS_PER_WORD 10        // Formatting
#define CHARS_PER_COL 31
#define WORD_PRECISION 16
#define VAR_UNITS_SEPARATOR "~"

// Variable Names
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
#define UTC "UTC"
#define UT1 "UT1"
#define TT "TT"

// Variable units
#define NO_UNIT "-"
#define KM "km"                  // Accepted units in header
#define M "m"                    // MUST NEVER BE LONGER THAN 10 CHARS
#define S "s"
#define MS "ms"
#define MS2 "ms^2"
#define KMS "km/s"
#define KMS2 "km/s^2"
#define KGM3 "kg/m^3"
#define SECONDS "s"
#define SECONDS_RELATIVE "s_rel"
#define MINUTES "mn"
#define HOURS "hr"
#define DAYS "d"
#define JD "JD"
#define MJD "MJD"
#define MJD2000 "MJD2000"

// ParameterEvolutionFile Types
#define NO_TYPE "UNSPECIFIED"
#define PEVF_TYPE_EPHM_NO_COV "EPHEM_NO_COV"

// ParameterEvolutionFile Sources
#define NO_SOURCE "UNSPECIFIED"
#define PEVF_TLEVISION_SGP4 "TLEVISION_SGP4"

// ParameterEvolutionFile References
#define NO_REFERENCE "UNSPECIFIED"
#define PEVF_REFERENCE_TEME "TEME"

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
    char* name;
    char* units;
    double* values;
    int n_values;
} ParameterEvolution;

/**
 * @brief
 * This struct contains a dataset (an array of ParameterEvolution structs) plus
 * some metadata, it can be directly written/read to/from a .pev file.
 * 
 * @param parameters An array of parameter evolutions.
 * @param n_parameters Number of parameter evolutions in parameters array.
 * @param filename Name of the file associated to (be) this data.
 * @param type Type of data. This is some context for the data inside parameters.
 * @param source Source of information.
 * @param reference Reference frame/system, where applicable.
 * @param comment Extra information useful to a human, not the computer.
 */
typedef struct {
    ParameterEvolution* parameters;
    int n_parameters;
    char* filename;
    char* type;
    char* source;
    char* reference;
    char* comment;
} ParameterEvolutionFile;

/**
 * @brief
 * Write an array of Parameter evolution arrays to a file.
 * 
 * @param p ParameterEvolutionFile contents + metadata to write to a file.
 */
StatusCode write_parameter_evolution_file(
    // Inputs
    const ParameterEvolutionFile p);

/**
 * @brief
 * Read an array of Parameter evolution arrays from a file.
 * 
 * @param out_p ParameterEvolutionFile object data.
 * @param filename file to read data from.
 */
StatusCode read_parameter_evolution_file(
        // Outputs
        ParameterEvolutionFile* out_p,
        // Inputs
        const char* filename);

/**
 * @brief
 * Search a ParameterEvolutionFile for a header, return the index of the header
 * 
 * @param header_index Index of header with name *header*
 * @param params Pointer to parameter evolution file object
 * @param header Header to search for
 * @return True if found, false otherwise
 */
bool parameter_evolution_file_find(
    // Outputs
    int *header_index,
    // Inputs
    const ParameterEvolutionFile *params,
    const char* header);

/**
 * @brief
 * Search a ParameterEvolutionFile for a header, return the index of the header
 *
 * @note out_timespan must be freed by the caller.
 * 
 * @param header_index Index of header with name *header*
 * @param params Pointer to parameter evolution file object
 * @param header Header to search for
 * @return True if found, false otherwise
 */
StatusCode parameter_evolution_file_get_jd(
    // Outputs
    ParameterEvolution* out_timespan,
    // Inputs
    const ParameterEvolutionFile *params);

/**
 * @brief 
 * Helper function - Remove trailing whitespace from a string.
 *
 * @param s The string to remove trailing whitespace from.
 */
void rtrim(char* s);

/**
 * @brief
 * Helper function - Returns the text after a given number of characters
 * within a string.
 * 
 * @param line The entire string
 * @param prefix_len The length of the prefix, all characters after this will be
 *                   returned.
 */
char* copy_after_prefix(char* line, int prefix_len);

#endif
