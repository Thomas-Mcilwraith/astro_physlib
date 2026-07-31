/*
 * parameter_evolution_file.c
 *
 * Author: Thomas McIlwraith
 * Date: 18/04/2026
 * url: /file-io/internal-products/parameter-evolution-file/parameter_evolution_file.c
 */

#include "parameter_evolution_file.h"

/**
 * Files are written with a fixed col with, see header file.
 * All Parameter evolutions must have the same number of data points, or else
 * the write will raise an error.
 * comment can be passed as NULL to write no comment.
 */
StatusCode parameter_evolution_file_write(
    // Inputs
    const parameter_evolution_file_t p,
    const char *working_directory) {

    // Local variables
    StatusCode status = OK;
    int i, j;
    char name_and_units[CHARS_PER_COL];
    char buffer[1024];

    if (!p.filename || !p.parameters || p.n_parameters <= 0) {
        LOG(ERROR, "Invalid ParameterEvolutionFile input");
        return ERROR;
    }

    for (i = 1; i < p.n_parameters; i++) {
        if (p.parameters[i].n_values != p.parameters[0].n_values) {
            LOG(ERROR,
                "All arrays written to a .pev must have the same length");
            return ERROR;
        }
    }

    // Define path to the parameter evolution file
    status = working_area_path(buffer, working_directory, FILES, p.filename, 1024);
    if (status != OK) {
        LOG(ERROR, "Failed to define path to workflow file");
        return ERROR;
    }

    FILE* fp = fopen(buffer, "w");
    if (!fp) {
        LOG(ERROR, "Failed to open file %s", p.filename);
        return ERROR;
    }

    if (p.type && p.type[0] != '\0') {
        fprintf(fp, "# TYPE: %s\n", p.type);
    } else {
        fprintf(fp, "# TYPE: %s\n", NO_TYPE);
    }

    if (p.source && p.source[0] != '\0') {
        fprintf(fp, "# SOURCE: %s\n", p.source);
    } else {
        fprintf(fp, "# SOURCE: %s\n", NO_SOURCE);
    }

    if (p.reference && p.reference[0] != '\0') {
        fprintf(fp, "# REFERENCE: %s\n", p.reference);
    } else {
        fprintf(fp, "# REFERENCE: %s\n", NO_REFERENCE);
    }

    if (p.comment && p.comment[0] != '\0') {
        fprintf(fp, "# COMMENT: %s\n", p.comment);
    } else {
        fprintf(fp, "# COMMENT: \n");
    }

    // Add an empty line between metadata and data
    fprintf(fp, "\n");

    // Write the header line
    for (i = 0; i < p.n_parameters; i++) {
        snprintf(name_and_units, sizeof(name_and_units), "%s%s%s",
                 p.parameters[i].name,
                 VAR_UNITS_SEPARATOR,
                 p.parameters[i].units);

        fprintf(fp, "%-*s", CHARS_PER_COL, name_and_units);
    }
    fprintf(fp, "\n");

    // Write each data line
    for (j = 0; j < p.parameters[0].n_values; j++) {
        for (i = 0; i < p.n_parameters; i++) {
            fprintf(fp, "%-*.*E",
                    CHARS_PER_COL,
                    WORD_PRECISION,
                    p.parameters[i].values[j]);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
    return OK;
}

/**
 * This function assumes the same conditions as write_parameter_evolution_file.
 * Only one comment line is permitted.
*/
StatusCode parameter_evolution_file_read(
        // Outputs
        parameter_evolution_file_t* out_p,
        // Inputs
        const char* filename,
        const char* working_directory) {

    // Local variables
    StatusCode status = OK;
    char buffer[1024];
    int i, line_len, n_params, capacity;
    int n_values = 0;

    if (!working_directory|| !filename) {
        LOG(ERROR, "Invalid input to read_parameter_evolution_file");
        return ERROR;
    }

    // Define path to the parameter evolution file
    status = working_area_path(buffer, working_directory, FILES, filename, 1024);
    if (status != OK) {
        LOG(ERROR, "Failed to define path to workflow file");
        return ERROR;
    }

    FILE* fp = fopen(buffer, "r");
    if (!fp) {
        LOG(ERROR, "Failed to open file %s", buffer);
        return ERROR;
    }

    memset(out_p, 0, sizeof(parameter_evolution_file_t));

    out_p->filename = NULL;
    out_p->type = NO_TYPE;
    out_p->source = NO_SOURCE;
    out_p->reference = NO_REFERENCE;
    out_p->comment = NULL;

    // Read metadata/comment lines.
    // Stop when we reach the first non-comment, non-empty line.
    // That line should be the header.
    while (fgets(buffer, sizeof(buffer), fp)) {

        rtrim(buffer);

        if (buffer[0] == '\0') {
            continue;
        }

        if (strncmp(buffer, "# TYPE:", 7) == 0) {
            out_p->type = copy_after_prefix(buffer, 7);
            if (!out_p->type) {
                fclose(fp);
                LOG(ERROR, "Failed to allocate memory for type");
                return ERROR;
            }
            continue;
        }

        if (strncmp(buffer, "# SOURCE:", 9) == 0) {
            out_p->source = copy_after_prefix(buffer, 9);
            if (!out_p->source) {
                fclose(fp);
                LOG(ERROR, "Failed to allocate memory for source");
                return ERROR;
            }
            continue;
        }

        if (strncmp(buffer, "# REFERENCE:", 12) == 0) {
            out_p->reference = copy_after_prefix(buffer, 12);
            if (!out_p->reference) {
                fclose(fp);
                LOG(ERROR, "Failed to allocate memory for reference");
                return ERROR;
            }
            continue;
        }

        if (strncmp(buffer, "# COMMENT:", 10) == 0) {
            out_p->comment = copy_after_prefix(buffer, 10);
            if (!out_p->comment) {
                fclose(fp);
                LOG(ERROR, "Failed to allocate memory for comment");
                return ERROR;
            }
            continue;
        }

        // TODO: Check if neccessary
        if (buffer[0] == '#') {
            continue;
        }

        break;
    }

    if (feof(fp)) {
        fclose(fp);
        LOG(ERROR, "No header line found in file %s", filename);
        return ERROR;
    }

    // The buffer now contains the header line. (minus trailing spaces)
    line_len = strlen(buffer);
    n_params = (line_len + CHARS_PER_COL - 1) / CHARS_PER_COL;

    if (n_params <= 0) {
        fclose(fp);
        free(out_p->comment);
        LOG(ERROR, "No parameters found in header");
        return ERROR;
    }

    parameter_evolution_t* params =
        malloc(n_params * sizeof(parameter_evolution_t));

    if (!params) {
        fclose(fp);
        free(out_p->comment);
        LOG(ERROR, "Failed to allocate memory for params");
        return ERROR;
    }

    memset(params, 0, n_params * sizeof(parameter_evolution_t));

    // Parse header columns.
    for (i = 0; i < n_params; i++) {

        char chunk[CHARS_PER_COL + 1];

        strncpy(chunk, buffer + i * CHARS_PER_COL, CHARS_PER_COL);
        chunk[CHARS_PER_COL] = '\0';

        char* sep = strstr(chunk, VAR_UNITS_SEPARATOR);

        if (!sep) {
            fclose(fp);
            free(params);
            free(out_p->comment);
            LOG(ERROR, "Separator not found in column %d", i);
            return ERROR;
        }

        *sep = '\0';

        char* name = chunk;
        char* units = sep + strlen(VAR_UNITS_SEPARATOR);

        rtrim(name);
        rtrim(units);

        params[i].name = malloc(strlen(name) + 1);
        params[i].units = malloc(strlen(units) + 1);

        if (!params[i].name || !params[i].units) {
            fclose(fp);

            for (int k = 0; k <= i; k++) {
                free(params[k].name);
                free(params[k].units);
            }

            free(params);
            free(out_p->comment);

            LOG(ERROR, "Failed to allocate memory for parameter metadata");
            return ERROR;
        }

        strcpy(params[i].name, name);
        strcpy(params[i].units, units);

        params[i].values = NULL;
        params[i].n_values = 0;
    }

    // Allocate initial value storage.
    capacity = 16;

    for (i = 0; i < n_params; i++) {
        params[i].values = malloc(capacity * sizeof(double));

        if (!params[i].values) {
            fclose(fp);

            for (int k = 0; k < n_params; k++) {
                free(params[k].name);
                free(params[k].units);
                free(params[k].values);
            }

            free(params);
            free(out_p->comment);

            LOG(ERROR, "Failed to allocate memory for parameter values");
            return ERROR;
        }
    }

    // Read data lines.
    while (fgets(buffer, sizeof(buffer), fp)) {

        rtrim(buffer);

        if (buffer[0] == '\0') {
            continue;
        }

        if (n_values >= capacity) {
            capacity *= 2;

            for (i = 0; i < n_params; i++) {
                double* tmp = realloc(
                    params[i].values,
                    capacity * sizeof(double)
                );

                if (!tmp) {
                    fclose(fp);

                    for (int k = 0; k < n_params; k++) {
                        free(params[k].name);
                        free(params[k].units);
                        free(params[k].values);
                    }

                    free(params);
                    free(out_p->comment);

                    LOG(ERROR, "Failed to reallocate parameter values");
                    return ERROR;
                }

                params[i].values = tmp;
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

    for (i = 0; i < n_params; i++) {
        params[i].n_values = n_values;
    }

    fclose(fp);

    out_p->parameters = params;
    out_p->n_parameters = n_params;

    return OK;
}

bool parameter_evolution_file_find(
    // Outputs
    int *header_index,
    // Inputs
    const parameter_evolution_file_t *params,
    const char* header) {

    // Local variables
    char input_header_trimmed[10], param_header_trimmed[10];

    // trim the input header
    strcpy(input_header_trimmed, header);
    rtrim(input_header_trimmed);

    for (int i = 0; i < params->n_parameters; i++){

        // trim the header for this parameter
        strcpy(param_header_trimmed, params->parameters[i].name);
        rtrim(param_header_trimmed);

        if (strcmp(input_header_trimmed, param_header_trimmed) == 0) {
            *header_index = i;
            return true;
        }
    }

    return false;
    }

StatusCode parameter_evolution_file_get_jd(
    // Outputs
    parameter_evolution_t* out_timespan,
    // Inputs
    const parameter_evolution_file_t *params) {

    // Local variables
    int time_header_index;
    bool found_time;

    *out_timespan = (parameter_evolution_t){0};

    out_timespan->n_values = params->parameters[0].n_values;
    out_timespan->values = malloc(out_timespan->n_values * sizeof(double));
    if (!out_timespan->values) {
        LOG(ERROR, "Failed to allocate memory for timespan");
        return ERROR;
    }

    // Search for all time representations
    found_time = parameter_evolution_file_find(&time_header_index, params, "UTC");
    if (!found_time) {
        found_time = parameter_evolution_file_find(&time_header_index, params, "UT1");
    }
    if (!found_time) {
        found_time = parameter_evolution_file_find(&time_header_index, params, "TT");
    }
    if (!found_time) {
        LOG(WARNING, "No timespan found in Parameter Evolution File");
        free(out_timespan->name);
        free(out_timespan->units);
        free(out_timespan->values);
        *out_timespan = (parameter_evolution_t){0};
        return WARNING;
    }

    out_timespan->name = strdup(params->parameters[time_header_index].name);

    // Convert the units to JD
    if (strcmp(params->parameters[time_header_index].units, MJD) == 0) {
        for (int i = 0; i < out_timespan->n_values; i++){
            out_timespan->values[i] = mjd_to_jd(
                    params->parameters[time_header_index].values[i]);
        }
    } else if (strcmp(params->parameters[time_header_index].units, MJD2000) == 0) {
        for (int i = 0; i < out_timespan->n_values; i++){
            out_timespan->values[i] = mjd2000_to_jd(
                    params->parameters[time_header_index].values[i]);
        }
    } else if (strcmp(params->parameters[time_header_index].units, JD) == 0) {
        for (int i = 0; i < out_timespan->n_values; i++){
            out_timespan->values[i] = params->parameters[time_header_index].values[i];
        }
    } else {
        LOG(ERROR, "Unexpected time units in Parameter Evolution File: %s",
                   params->parameters[time_header_index].units);
        free(out_timespan->name);
        free(out_timespan->units);
        free(out_timespan->values);
        *out_timespan = (parameter_evolution_t){0};
        return ERROR;
    }

    out_timespan->units = strdup(JD);

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

/**
 * Helper Function: returns the text after a given prefix within a string.
 */
char* copy_after_prefix(char* line, int prefix_len) {
    char* value = line + prefix_len;

    while (*value == ' ') {
        value++;
    }

    char* out = malloc(strlen(value) + 1);
    if (!out) {
        return NULL;
    }

    strcpy(out, value);
    return out;
}
