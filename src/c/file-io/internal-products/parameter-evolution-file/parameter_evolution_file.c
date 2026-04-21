/*
 * parameter_evolution_file.c
 *
 * Author: Thomas McIlwraith
 * Date: 18/04/2026
 * url: /file-io/internal-products/parameter-evolution-file/parameter_evolution_file.c
 */

#include "parameter_evolution_file.h"

/**
 * See two_body_problem.h for IO description.
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
    char name_units[CHARS_PER_WORD];

    // Check that all ParameterToWrite structs have the same number of values
    for (i = 1; i < n_params; i++) {
        if (params[i].n_values != params[0].n_values) {
            LOG("ERROR",
                    "All arrays written to a .pef must have the same length");
            return ERROR;
        }
    }

    // Open the file in append mode
    FILE* fp = fopen(filename, "a");
    if (fp == NULL){
        LOG("ERROR", "Failed to open file %s", filename);
        return ERROR;
    }

    // Write the header line - params are in the format VAR_NAME~m/s^2
    for (i = 0; i < n_params; i++) {
        // first, format the name and units
        strcpy(name_units, params[i].name);
        strcat(name_units, VAR_UNITS_SEPARATOR);
        strcat(name_units, params[i].units);
        fprintf(fp, "%-*s", CHARS_PER_WORD, name_units);
    }
    fprintf(fp, "\n");

    // Write the data line(s)
    for (j = 0; j < params[0].n_values; j++) {
        for (i = 0; i < n_params; i++) {
            fprintf(fp, "%-*.*e",
                    CHARS_PER_WORD, WORD_PRECISION, params[i].values[j]);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
    return OK;
}

int main(){

    const char* filename = "test.txt";
    double rho[5] = {6378138.145375, 2.0e16, 3.0, 4.0, 5.0};
    double acc[5] = {1.0, 2.0, 3.0, 4.0, 5.0};
    double quat[5] = {1.0, 2.0, 3.0, 4.0, 5.0};

    ParameterEvolution params[3] = {
            {RHO, KGM3, rho, 5},
            {ACC, MS2, acc, 5},
            {QUAT, NO_UNIT, quat, 5}
    };

    write_parameter_evolution_file(filename, params, 3);

    return 0;
}

