/*
 * tlevision.c
 *
 * Author: Thomas McIlwraith
 * Date: 63/07/2026
 * 
 * Program for generating ephemerides from TLEs.
 * 
 */

#include "file-io/internal-products/parameter-evolution-file/parameter_evolution_file.h"
#include "file-io/internal-products/read-json/read_json.h"
#include "file-io/data_structures/tle/tle.h"
#include "tlevision/tlevision_generate_arc.h"
// #include "tlevision_interfaces.h"
#include "utilities/misc/paths/paths.h"


typedef struct {

    char *parent_node_id;
    char *name;
    char *type;
    size_t id;

} workflow_port_t;

typedef struct {

    char *id;
    char *program;

    workflow_port_t *inputs;
    size_t n_inputs;

    workflow_port_t *outputs;
    size_t n_outputs;

} workflow_node_t;

typedef struct {

    size_t from;
    size_t to;

} workflow_edge_t;

typedef struct {

    char *workflow_id;

    workflow_node_t *nodes;
    size_t n_nodes;

    workflow_edge_t *edges;
    size_t n_edges;

} workflow_graph_t;

StatusCode load_workflow_graph(workflow_graph_t *graph, const ExecutionSettings execution_settings) {

    // Local variables
    StatusCode status = OK;
    cJSON *workflow_json;
    char workflow_filepath[1024];

    // Define path to the graph file
    status = working_area_path(workflow_filepath, execution_settings.working_directory, INPUTS, "graph.json", 1024);
    if (status != OK) {
        LOG(ERROR, "Failed to define path to workflow file");
        return ERROR;
    }

    status = read_json(&workflow_json, workflow_filepath);
    if (status != OK) {
        LOG(ERROR, "Failed to read workflow JSON file");
        return ERROR;
    }

    char *workflow_id = cJSON_GetObjectItemCaseSensitive(workflow_json, "workflow")->valuestring;
    if (!workflow_id) {
        LOG(ERROR, "Malformed workflow JSON file: workflow_id");
        cJSON_Delete(workflow_json);
        return ERROR;
    }

    const cJSON *graph_json = cJSON_GetObjectItemCaseSensitive(workflow_json, "graph");
    if (!graph_json) {
        LOG(ERROR, "Malformed workflow JSON file: graph");
        cJSON_Delete(workflow_json);
        return ERROR;
    }

    const cJSON *nodes_json = cJSON_GetObjectItemCaseSensitive(graph_json, "nodes");
    if (!nodes_json) {
        LOG(ERROR, "Malformed workflow JSON file: nodes");
        cJSON_Delete(workflow_json);
        return ERROR;
    }

    const cJSON *edges_json = cJSON_GetObjectItemCaseSensitive(graph_json, "edges");
    if (!edges_json) {
        LOG(ERROR, "Malformed workflow JSON file: edges");
        cJSON_Delete(workflow_json);
        return ERROR;
    }

    const int n_nodes = cJSON_GetArraySize(nodes_json);
    const int n_edges = cJSON_GetArraySize(edges_json);

    // Now we know how much to allocate, so allocate memory for nodes and edges
    graph->workflow_id = strdup(workflow_id);
    if (graph->workflow_id == NULL) {
        LOG(ERROR, "Failed to allocate memory: workflow_id");
        cJSON_Delete(workflow_json);
        return ERROR;
    }
    graph->nodes = malloc(n_nodes * sizeof(workflow_node_t));
    if (graph->nodes == NULL) {
        LOG(ERROR, "Failed to allocate memory: nodes");
        cJSON_Delete(workflow_json);
        return ERROR;
    }
    graph->edges = malloc(n_edges * sizeof(workflow_edge_t));
    if (graph->nodes == NULL) {
        LOG(ERROR, "Failed to allocate memory: edges");
        cJSON_Delete(workflow_json);
        return ERROR;
    }
    graph->n_nodes = n_nodes;
    graph->n_edges = n_edges;

    // Add nodes to the graph
    for (int i = 0; i < n_nodes; i++) {
        cJSON *node_json = cJSON_GetArrayItem(nodes_json, i);
        if (!node_json) {
            LOG(ERROR, "Malformed workflow JSON file: nodes[%d]", i);
            cJSON_Delete(workflow_json);
            return ERROR;
        }

        char* node_id = cJSON_GetObjectItemCaseSensitive(node_json, "id")->valuestring;
        if (!node_id) {
            LOG(ERROR, "Malformed workflow JSON file: node_id");
            cJSON_Delete(workflow_json);
            return ERROR;
        }

        char* node_program = cJSON_GetObjectItemCaseSensitive(node_json, "program")->valuestring;
        if (!node_program) {
            LOG(ERROR, "Malformed workflow JSON file: node_program");
            cJSON_Delete(workflow_json);
            return ERROR;
        }

        cJSON *node_ports_json = cJSON_GetObjectItemCaseSensitive(node_json, "ports");
        if (!node_ports_json) {
            LOG(ERROR, "Malformed workflow JSON file: node_ports");
            cJSON_Delete(workflow_json);
            return ERROR;
        }

        cJSON *node_inputs_json = cJSON_GetObjectItemCaseSensitive(node_ports_json, "inputs");
        if (!node_inputs_json) {
            LOG(ERROR, "Malformed workflow JSON file: node_inputs");
            cJSON_Delete(workflow_json);
            return ERROR;
        }

        cJSON *node_outputs_json = cJSON_GetObjectItemCaseSensitive(node_ports_json, "outputs");
        if (!node_outputs_json) {
            LOG(ERROR, "Malformed workflow JSON file: node_outputs");
            cJSON_Delete(workflow_json);
            return ERROR;
        }

        int n_inputs = cJSON_GetArraySize(node_inputs_json);
        int n_outputs = cJSON_GetArraySize(node_outputs_json);
        LOG(INFO, "Node %s has %d inputs and %d outputs", node_id, n_inputs, n_outputs);

        // Now we know how many ports this node has, so allocate memory
        graph->nodes[i].id = strdup(node_id);
        if (graph->nodes[i].id == NULL) {
            LOG(ERROR, "Failed to allocate memory: node[%d].id", i);
            cJSON_Delete(workflow_json);
            return ERROR;
        }
        graph->nodes[i].program = strdup(node_program);
        if (graph->nodes[i].program == NULL) {
            LOG(ERROR, "Failed to allocate memory: node[%d].program", i);
            cJSON_Delete(workflow_json);
            return ERROR;
        }
        graph->nodes[i].n_inputs = n_inputs;
        graph->nodes[i].n_outputs = n_outputs;
        graph->nodes[i].inputs = malloc(n_inputs * sizeof(workflow_port_t));
        if (graph->nodes[i].inputs == NULL) {
            LOG(ERROR, "Failed to allocate memory: node[%d].inputs", i);
            cJSON_Delete(workflow_json);
            return ERROR;
        }
        graph->nodes[i].outputs = malloc(n_outputs * sizeof(workflow_port_t));
        if (graph->nodes[i].outputs == NULL) {
            LOG(ERROR, "Failed to allocate memory: node[%d].outputs", i);
            cJSON_Delete(workflow_json);
            return ERROR;
        }

        // Add inputs to the node
        for (int j = 0; j < n_inputs; j++) {
            cJSON *input_json = cJSON_GetArrayItem(node_inputs_json, j);
            if (!input_json) {
                LOG(ERROR, "Malformed workflow JSON file: node_inputs[%d]", j);
                cJSON_Delete(workflow_json);
                return ERROR;
            }

            cJSON *input_id = cJSON_GetObjectItemCaseSensitive(input_json, "id");
            if (!cJSON_IsNumber(input_id)) {
                LOG(ERROR, "Malformed workflow JSON file: nodes[%d].ports[%d].inputs[%d].id", i, j);
                cJSON_Delete(workflow_json);
                return ERROR;
            }
            int input_id_value = input_id->valueint;

            char *input_name = cJSON_GetObjectItemCaseSensitive(input_json, "name")->valuestring;
            if (!input_name) {
                LOG(ERROR, "Malformed workflow JSON file: nodes[%d].ports[%d].inputs[%d].name", i, j);
                cJSON_Delete(workflow_json);
                return ERROR;
            }

            char *input_type = cJSON_GetObjectItemCaseSensitive(input_json, "type")->valuestring;
            if (!input_type) {
                LOG(ERROR, "Malformed workflow JSON file: nodes[%d].ports[%d].inputs[%d].type", i, j);
                cJSON_Delete(workflow_json);
                return ERROR;
            }

            graph->nodes[i].inputs[j].id = input_id_value;
            graph->nodes[i].inputs[j].name = strdup(input_name);
            if (graph->nodes[i].inputs[j].name == NULL) {
                LOG(ERROR, "Failed to allocate memory: nodes[%d].ports[%d].inputs[%d].name", i, j);
                cJSON_Delete(workflow_json);
                return ERROR;
            }
            graph->nodes[i].inputs[j].type = strdup(input_type);
            if (graph->nodes[i].inputs[j].type == NULL) {
                LOG(ERROR, "Failed to allocate memory: nodes[%d].ports[%d].inputs[%d].type", i, j);
                cJSON_Delete(workflow_json);
                return ERROR;
            }
        }

        for (int k = 0; k < n_outputs; k++) {
            cJSON *output_json = cJSON_GetArrayItem(node_outputs_json, k);
            if (!output_json) {
                LOG(ERROR, "Malformed workflow JSON file: node_outputs[%d]", k);
                cJSON_Delete(workflow_json);
                return ERROR;
            }

            cJSON *output_id = cJSON_GetObjectItemCaseSensitive(output_json, "id");
            if (!cJSON_IsNumber(output_id)) {
                LOG(ERROR, "Malformed workflow JSON file: nodes[%d].ports[%d].outputs[%d].id", i, k);
                cJSON_Delete(workflow_json);
                return ERROR;
            }
            int output_id_value = output_id->valueint;

            char *output_name = cJSON_GetObjectItemCaseSensitive(output_json, "name")->valuestring;
            if (!output_name) {
                LOG(ERROR, "Malformed workflow JSON file: nodes[%d].ports[%d].outputs[%d].name", i, k);
                cJSON_Delete(workflow_json);
                return ERROR;
            }

            char *output_type = cJSON_GetObjectItemCaseSensitive(output_json, "type")->valuestring;
            if (!output_type) {
                LOG(ERROR, "Malformed workflow JSON file: nodes[%d].ports[%d].outputs[%d].type", i, k);
                cJSON_Delete(workflow_json);
                return ERROR;
            }

            graph->nodes[i].outputs[k].id = output_id_value;
            graph->nodes[i].outputs[k].name = strdup(output_name);
            if (graph->nodes[i].outputs[k].name == NULL) {
                LOG(ERROR, "Failed to allocate memory: nodes[%d].ports[%d].outputs[%d].name", i, k);
                cJSON_Delete(workflow_json);
                return ERROR;
            }
            graph->nodes[i].outputs[k].type = strdup(output_type);
            if (graph->nodes[i].outputs[k].type == NULL) {
                LOG(ERROR, "Failed to allocate memory: nodes[%d].ports[%d].outputs[%d].type", i, k);
                cJSON_Delete(workflow_json);
                return ERROR;
            }
        }
    }

    // Free the JSON
    cJSON_Delete(workflow_json);

    return OK;
}

int main(int argc, char *argv[]) {
    // // Outputs
    // application_output_t tlevision_output = {0};
    // ParameterEvolutionFile *ephm_output = {0};
    // int n_ephm_output = 0;
    // // Inputs
    // tlevision_input_t tlevision_input = {0};
    const char* program_name = "tlevision";
    ExecutionSettings execution_settings = {0};
    // application_output_t tspn_output = {0};
    // application_output_t a_tle_output = {0};
    //
    // // Program variables
    StatusCode status = OK;
    workflow_graph_t graph;
    // const char* database_tle_file = "/home/admin/test_spacetrack_tle_cat.json";  // TODO: Retrieve from database
    // char output_filepath_buf[1024];
    // cJSON *tle_cat;
    // ParameterEvolution tspn = {0};
    // tle_t *a_tle = {0};
    // int n_tles = 0;
    //
    // Parse command line, initialise log file
    status = parse_cmdline(&execution_settings, argc, argv);
    if (status != OK) {
        LOG(ERROR, "Failed to parse command line");
        return ERROR;
    }
    init_log(execution_settings.run_title, execution_settings.working_directory, program_name);

    // Load the workflow graph
    status = load_workflow_graph(&graph, execution_settings);
    if (status != OK) {
        LOG(ERROR, "Failed to load workflow graph");
        return ERROR;
    }
    //
    // // Read the input file
    // status = read_tlevision_inputs(&tlevision_input, execution_settings.working_directory, execution_settings.run_title, program_name);
    // if (status != OK) {
    //     LOG(ERROR, "Failed to read Inputs");
    //     return ERROR;
    // }
    //
    // // If timespan is loaded from another program. Load that output here.
    // if (tlevision_input.tspn.source == TSPN_SOURCE_PROGRAM) {
    //     status = application_output_read_json(&tspn_output, execution_settings.working_directory, tlevision_input.tspn.source_program_id, tlevision_input.tspn.source_program_name);
    //     if (status != OK) {
    //         LOG(ERROR, "Failed to read program output: %s %s", tlevision_input.tspn.source_program_id, tlevision_input.tspn.source_program_name);
    //         return ERROR;
    //     }
    // }
    //
    // // If the TLE is loaded from another program, load that output here.
    // // Since TLEs must be source from the same program, assume first element
    // if (tlevision_input.a_tle[0].source == TLE_SOURCE_PROGRAM) {
    //     status = application_output_read_json(&a_tle_output, execution_settings.working_directory, tlevision_input.a_tle[0].source_program_id, tlevision_input.a_tle[0].source_program_name);
    //     if (status != OK) {
    //         LOG(ERROR, "Failed to read program output: %s %s", tlevision_input.a_tle[0].source_program_id, tlevision_input.a_tle[0].source_program_name);
    //         return ERROR;
    //     }
    // }
    //
    // // If the TLE is to be loaded from the catalog, load the catalog here.
    // if (tlevision_input.a_tle[0].source == TLE_SOURCE_CATALOG) {
    //     LOG(INFO, "Loading SpaceTrack TLE catalogue");
    //     status = read_json(&tle_cat, database_tle_file);
    //     if (status != OK) {
    //         LOG(ERROR, "Failed to read JSON file: %s", database_tle_file);
    //         return ERROR;
    //     }
    // }
    //
    // // Load the timespan
    // LOG(INFO, "Loading TSPN into object");
    // status = application_inputs_tspn_load(&tspn, &tlevision_input.tspn, &tspn_output, &execution_settings, program_name);
    // if (status != OK) {
    //     LOG(ERROR, "Failed to load timespan");
    //     return ERROR;
    // }
    // LOG(INFO, "Loaded TSPN object successfully");
    //
    // // Load the TLEs
    // LOG(INFO, "Loading TLEs into object(s)");
    // status = tle_load(&a_tle, &n_tles, tlevision_input.a_tle, tlevision_input.n_tle, &a_tle_output, &execution_settings, tle_cat);
    // if (status != OK) {
    //     LOG(ERROR, "Failed to load TLEs");
    //     return ERROR;
    // }
    // LOG(INFO, "Loaded TLEs successfully", n_tles);
    //
    // // Generate the ephemeris with SGP4 model
    // LOG(INFO, "Generating ephemerides with SGP4");
    // status = ephm_generate_SGP4(&ephm_output, &n_ephm_output, &tspn, a_tle, n_tles, tlevision_input.wgs_model, execution_settings.run_title);
    // if (status != OK) {
    //     LOG(ERROR, "Failed to generate ephemerides");
    //     return ERROR;
    // }
    // LOG(INFO, "Generated ephemerides successfully");
    //
    // // Write all the output files
    // LOG(INFO, "Generating Outputs");
    // for (int i = 0; i < n_ephm_output; i++) {
    //     // Define the file path for this file
    //     status = working_area_path(output_filepath_buf, execution_settings.working_directory, FILES, "test", 1024);
    //     status = write_parameter_evolution_file(ephm_output[i]);
    //     if (status == OK) {
    //         LOG(INFO, "Output File: %s", ephm_output[i].filename);
    //     } else {
    //         LOG(ERROR, "Failed to write ephemeris file");
    //         return ERROR;
    //     }
    // }
    //
    // LOG(INFO, "Program complete: %s (%s)", program_name, execution_settings.run_title);
    //
    // // Free memory
    // if (tlevision_input.a_tle[0].source == TLE_SOURCE_CATALOG) cJSON_Delete(tle_cat);
    // if (tlevision_input.tspn.source == TSPN_SOURCE_PROGRAM) application_output_free(&tspn_output);
    // if (tlevision_input.a_tle[0].source == TLE_SOURCE_PROGRAM) application_output_free(&a_tle_output);
    // free (a_tle);
    // close_log();
    //
    return OK;
}

