/*
 * tlevision.c
 *
 * Author: Thomas McIlwraith
 * Date: 30/07/2026
 * 
 */

#include "workflow_graph.h"

StatusCode workflow_port_read(
    // Outputs
    workflow_port_t *port,
    // Inputs
    const cJSON *port_json) {

    cJSON *input_id = cJSON_GetObjectItemCaseSensitive(port_json, "id");
    if (!cJSON_IsNumber(input_id)) {
        LOG(ERROR, "Malformed workflow JSON file: port.id");
        return ERROR;
    }
    int input_id_value = input_id->valueint;

    char *input_name = cJSON_GetObjectItemCaseSensitive(port_json, "name")->valuestring;
    if (!input_name) {
        LOG(ERROR, "Malformed workflow JSON file: port.name");
        return ERROR;
    }

    char *input_type = cJSON_GetObjectItemCaseSensitive(port_json, "type")->valuestring;
    if (!input_type) {
        LOG(ERROR, "Malformed workflow JSON file: port.type");
        return ERROR;
    }

    port->id = input_id_value;
    port->name = strdup(input_name);
    if (port->name == NULL) {
        LOG(ERROR, "Failed to allocate memory: port.name");
        return ERROR;
    }
    port->type = strdup(input_type);
    if (port->type == NULL) {
        LOG(ERROR, "Failed to allocate memory: port.type");
        return ERROR;
    }

    return OK;
}

StatusCode workflow_node_read(
    // Outputs
    workflow_node_t *node,
    // Inputs
    const cJSON *node_json) {

    // Local variables
    StatusCode status = OK;

    char* node_id = cJSON_GetObjectItemCaseSensitive(node_json, "id")->valuestring;
    if (!node_id) {
        LOG(ERROR, "Malformed workflow JSON file: node_id");
        return ERROR;
    }

    char* node_program = cJSON_GetObjectItemCaseSensitive(node_json, "program")->valuestring;
    if (!node_program) {
        LOG(ERROR, "Malformed workflow JSON file: node_program");
        return ERROR;
    }

    cJSON *node_ports_json = cJSON_GetObjectItemCaseSensitive(node_json, "ports");
    if (!node_ports_json) {
        LOG(ERROR, "Malformed workflow JSON file: node_ports");
        return ERROR;
    }

    cJSON *node_inputs_json = cJSON_GetObjectItemCaseSensitive(node_ports_json, "inputs");
    if (!node_inputs_json) {
        LOG(ERROR, "Malformed workflow JSON file: node_inputs");
        return ERROR;
    }

    cJSON *node_outputs_json = cJSON_GetObjectItemCaseSensitive(node_ports_json, "outputs");
    if (!node_outputs_json) {
        LOG(ERROR, "Malformed workflow JSON file: node_outputs");
        return ERROR;
    }

    int n_inputs = cJSON_GetArraySize(node_inputs_json);
    int n_outputs = cJSON_GetArraySize(node_outputs_json);

    // Now we know how many ports this node has, so allocate memory
    node->id = strdup(node_id);
    if (node->id == NULL) {
        LOG(ERROR, "Failed to allocate memory: node.id");
        return ERROR;
    }
    node->program = strdup(node_program);
    if (node->program == NULL) {
        LOG(ERROR, "Failed to allocate memory: node.program");
        return ERROR;
    }
    node->n_inputs = n_inputs;
    node->n_outputs = n_outputs;
    node->inputs = malloc(n_inputs * sizeof(workflow_port_t));
    if (node->inputs == NULL) {
        LOG(ERROR, "Failed to allocate memory: node.inputs");
        return ERROR;
    }
    node->outputs = malloc(n_outputs * sizeof(workflow_port_t));
    if (node->outputs == NULL) {
        LOG(ERROR, "Failed to allocate memory: node.outputs");
        return ERROR;
    }

    // Add inputs to the node
    for (int j = 0; j < n_inputs; j++) {
        cJSON *node_input = cJSON_GetArrayItem(node_inputs_json, j);
        if (!node_input) {
            LOG(ERROR, "Malformed workflow JSON file: node_inputs[%d]", j);
            return ERROR;
        }

        status = workflow_port_read(&node->inputs[j], node_input);
        if (status != OK) {
            LOG(ERROR, "Failed to read workflow port: %d", j);
            return ERROR;
        }
    }

    // Add outputs to the node
    for (int k = 0; k < n_outputs; k++) {
        cJSON *node_output = cJSON_GetArrayItem(node_outputs_json, k);
        if (!node_output) {
            LOG(ERROR, "Malformed workflow JSON file: node_outputs[%d]", k);
            return ERROR;
        }

        status = workflow_port_read(&node->outputs[k], node_output);
        if (status != OK) {
            LOG(ERROR, "Failed to read workflow port: %d", k);
            return ERROR;
        }
    }

    return OK;
}

StatusCode workflow_edge_read(
    // Outputs
    workflow_edge_t *edge,
    // Inputs
    const cJSON *edge_json) {

    cJSON *edge_from = cJSON_GetObjectItemCaseSensitive(edge_json, "from");
    if (!edge_from) {
        LOG(ERROR, "Malformed workflow JSON file: edge.from");
        return ERROR;
    }
    int edge_from_value = edge_from->valueint;

    cJSON *edge_to = cJSON_GetObjectItemCaseSensitive(edge_json, "to");
    if (!edge_to) {
        LOG(ERROR, "Malformed workflow JSON file: edge.to");
        return ERROR;
    }
    int edge_to_value = edge_to->valueint;

    edge->from = edge_from_value;
    edge->to = edge_to_value;

    return OK;
}

StatusCode workflow_graph_read(
    // Outputs
    workflow_graph_t *graph,
    workflow_node_t **this_node,
    // Inputs
    const ExecutionSettings execution_settings) {

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
    graph->nodes = malloc(n_nodes * sizeof(workflow_node_t));
    if (graph->nodes == NULL) {
        LOG(ERROR, "Failed to allocate memory: nodes");
        cJSON_Delete(workflow_json);
        return ERROR;
    }
    graph->edges = malloc(n_edges * sizeof(workflow_edge_t));
    if (graph->edges == NULL) {
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

        status = workflow_node_read(&graph->nodes[i], node_json);
        if (status != OK) {
            LOG(ERROR, "Failed to read workflow node: %d", i);
            cJSON_Delete(workflow_json);
            return ERROR;
        }

        // Check if this node is the program we are currently in
        if (strcmp(graph->nodes[i].id, execution_settings.run_title) == 0) {

            // If yes, return the pointer this node to the caller
            *this_node = &graph->nodes[i];
        }
    }

    // Add edges to the graph
    for (int j = 0; j < n_edges; j++) {
        cJSON *edge_json = cJSON_GetArrayItem(edges_json, j);
        if (!edge_json) {
            LOG(ERROR, "Malformed workflow JSON file: edges[%d]", j);
            cJSON_Delete(workflow_json);
            return ERROR;
        }

        status = workflow_edge_read(&graph->edges[j], edge_json);
        if (status != OK) {
            LOG(ERROR, "Failed to read workflow edge: %d", j);
            cJSON_Delete(workflow_json);
            return ERROR;
        }
    }

    // Check that the current program was found in the graph
    if (*this_node == NULL) {
        LOG(ERROR, "Current program not found in workflow graph");
        cJSON_Delete(workflow_json);
        return ERROR;
    }

    // Free the JSON
    cJSON_Delete(workflow_json);

    return OK;
}

