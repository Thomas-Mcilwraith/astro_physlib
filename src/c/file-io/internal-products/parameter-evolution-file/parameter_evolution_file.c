/*
 * parameter_evolution_file.c
 *
 * Author: Thomas McIlwraith
 * Date: 18/04/2026
 * url: /file-io/internal-products/parameter-evolution-file/parameter_evolution_file.c
 */

#include "parameter_evolution_file.h"
#include "utilities/logging/log/log.h"

/**
 * See parameter_evolution_file.h for IO description.
 * 
 * Files are written with a fixed col with, see header file.
 * All Parameter evolutions must have the same number of data points, or else
 * the write will raise an error.
 */
StatusCode write_parameter_evolution_file(
        // Inputs
        const char* filename,
        const ParameterEvolution* params,
        const int n_params){

    // Local variables
    int i, j;
    char name_units[CHARS_PER_COL];

    // Check that all ParameterToWrite structs have the same number of values
    for (i = 1; i < n_params; i++) {
        if (params[i].n_values != params[0].n_values) {
            LOG("ERROR",
                    "All arrays written to a .pef must have the same length");
            return ERROR;
        }
    }

    // Open the file in write mode
    FILE* fp = fopen(filename, "w");
    if (!fp){
        LOG("ERROR", "Failed to open file %s", filename);
        return ERROR;
    }

    // Write the header line - params are in the format VAR_NAME~m/s^2
    for (i = 0; i < n_params; i++) {
        // first, format the name and units
        strcpy(name_units, params[i].name);
        strcat(name_units, VAR_UNITS_SEPARATOR);
        strcat(name_units, params[i].units);
        fprintf(fp, "%-*s", CHARS_PER_COL, name_units);
    }
    fprintf(fp, "\n");

    // Write the data line(s)
    for (j = 0; j < params[0].n_values; j++) {
        for (i = 0; i < n_params; i++) {
            fprintf(fp, "%-*.*e",
                    CHARS_PER_COL, WORD_PRECISION, params[i].values[j]);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
    return OK;
}

/**
 * See parameter_evolution_file.h for IO description.
 *
 * This function assumes the same conditions as write_parameter_evolution_file.
 * I.e. any file attempted to be read must have the EXACT SAME FORMAT as what
 * write_parameter_evolution_file writes. Addition spaces, newlines, etc. will
 * cause the read to fail.
*/
StatusCode read_parameter_evolution_file(ParameterEvolution** out_params, 
        int* out_n_params,
        int* out_n_values,
        // Inputs
        const char* filename) {

    // Local variables
    char buffer[1024];
    int i, line_len, n_params, capacity;
    int n_values = 0;

    FILE* fp = fopen(filename, "r");
    if (!fp) {
        LOG("ERROR", "Failed to open file %s", filename);
        return ERROR;
    }

    // Read the header into the buffer
    if (!fgets(buffer, sizeof(buffer), fp)) {
        fclose(fp);
        LOG("ERROR", "Failed to read header from file %s", filename);
        return ERROR;
    }

    // Determine the number of parameters
    line_len = strlen(buffer);
    n_params = line_len / CHARS_PER_COL;
    ParameterEvolution* params =
        malloc(n_params * sizeof(ParameterEvolution));

    if (!params) {
        fclose(fp);
        LOG("ERROR", "Failed to allocate memory for params");
        return ERROR;
    }

    // Parse header columns
    for (i = 0; i < n_params; i++) {
        // Add one for the null terminator
        char chunk[CHARS_PER_COL + 1];
        // Copy the column into chunk
        strncpy(chunk, buffer + i * CHARS_PER_COL, CHARS_PER_COL);
        chunk[CHARS_PER_COL] = '\0';

        // strstr() returns a pointer to the first occurrence of the separator
        char* sep = strstr(chunk, VAR_UNITS_SEPARATOR);

        if (!sep) {
            fclose(fp);
            free(params);
            LOG("ERROR", "Separator not found in column %d", i);
            return ERROR;
        }

        // By forcing the separator to null terminator, we effectively split
        // the string into two parts (chunk="name", sep+1="units)"
        *sep = '\0';
        char* name = chunk;
        char* units = sep + strlen(VAR_UNITS_SEPARATOR);

        // remove trailing whitespace
        rtrim(name);
        rtrim(units);

        strcpy(params[i].name, name);
        strcpy(params[i].units, units);

        params[i].values = NULL;
        params[i].n_values = 0;
    }

    // Now we know the number of parameters but need to read a number of data
    // points which we do not know.
    // Start with an initial guess of 16 (lines) and double it until we get to
    // the end of the file
    capacity = 16;
    for (i = 0; i < n_params; i++) {
        params[i].values = malloc(capacity * sizeof(double));
    }

    // Until we reach the end of the file, continue reading lines and if
    // the number of lines read (n_values) exceeds the capacity, double the
    // capacity and reallocate the memory
    while (fgets(buffer, sizeof(buffer), fp)) {

        if (n_values >= capacity) {
            capacity *= 2;
            for (i = 0; i < n_params; i++) {
                params[i].values = realloc(
                    params[i].values,
                    capacity * sizeof(double)
                );
            }
        }

        for (i = 0; i < n_params; i++) {
            char chunk[CHARS_PER_COL + 1];
            strncpy(chunk, buffer + i * CHARS_PER_COL, CHARS_PER_COL);
            chunk[CHARS_PER_COL] = '\0';

            params[i].values[n_values] = atof(chunk);
        }

        n_values++;
    }

    // Finalise lengths
    for (i = 0; i < n_params; i++) {
        params[i].n_values = n_values;
    }

    fclose(fp);
    *out_params = params;
    *out_n_params = n_params;
    *out_n_values = n_values;

    return OK;
}

/**
 * Helper function - Remove trailing whitespace from a string.
 */
void rtrim(char* s) {
    int len = strlen(s);
    while (len > 0 && (s[len - 1] == ' ' || s[len - 1] == '\n')) {
        s[len - 1] = '\0';
        len--;
    }
}

