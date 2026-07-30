#ifndef WORKFLOW_GRAPH_H
#define WORKFLOW_GRAPH_H

// Standard libraries
#include <stdlib.h>
#include "external/cjson/cJSON.h"
#include "file-io/internal-products/read-json/read_json.h"
#include "utilities/logging/log/log.h"
#include "utilities/misc/parse-cmdline/parse_cmdline.h"

// Variable and Macro definitions

// Function prototypes

/**
 * @brief Data structure for a port object within the workflow graph
 * 
 * @param parent_node_id The ID of the parent node
 * @param name The name of the port
 * @param type The type of the port
 * @param id The ID of the port
 */
typedef struct {

    char *parent_node_id;
    char *name;
    char *type;
    size_t id;

} workflow_port_t;

/**
 * @brief Data structure for a node object within the workflow graph
 * 
 * @param id The ID of the node
 * @param program The program that the node runs
 * @param inputs The inputs to the node
 * @param n_inputs The number of inputs to the node
 * @param outputs The outputs from the node
 * @param n_outputs The number of outputs from the node
 */
typedef struct {

    char *id;
    char *program;

    workflow_port_t *inputs;
    size_t n_inputs;

    workflow_port_t *outputs;
    size_t n_outputs;

} workflow_node_t;

/**
 * @brief Data structure for an edge object within the workflow graph
 * 
 * @param from The ID of the node that the edge starts at
 * @param to The ID of the node that the edge ends at
 */
typedef struct {

    size_t from;
    size_t to;

} workflow_edge_t;

/**
 * @brief Data structure for the workflow graph
 * 
 * @param workflow_id The ID of the workflow
 * @param nodes The nodes in the workflow graph
 * @param n_nodes The number of nodes in the workflow graph
 * @param edges The edges in the workflow graph
 * @param n_edges The number of edges in the workflow graph
 */
typedef struct {

    workflow_node_t *nodes;
    size_t n_nodes;

    workflow_edge_t *edges;
    size_t n_edges;

} workflow_graph_t;

/**
 * @brief Reads a port cJSON object into a workflow_port_t object
 * 
 * @param port The port object to read into
 * @param port_json The cJSON object to read from
 */
StatusCode workflow_port_read(
    // Outputs
    workflow_port_t *port,
    // Inputs
    const cJSON *port_json);

/**
 * @brief Reads a node cJSON object into a workflow_node_t object
 * 
    size_t *connected_port,
 * @param node The node object to read into
 * @param node_json The cJSON object to read from
 */
StatusCode workflow_node_read(
    // Outputs
    workflow_node_t *node,
    // Inputs
    const cJSON *node_json);

/**
 * @brief Reads an edge cJSON object into a workflow_edge_t object
 * 
 * @param edge The edge object to read into
 * @param edge_json The cJSON object to read from
 */
StatusCode workflow_edge_read(
    // Outputs
    workflow_edge_t *edge,
    // Inputs
    const cJSON *edge_json);

/**
 * @brief Reads a workflow graph cJSON object into a workflow_graph_t object
 * 
 * @param graph The graph object to read into
 * @param this_node A pointer to the node of the program is currently running
 * @param execution_settings The execution settings object
 */
StatusCode workflow_graph_read(
    // Outputs
    workflow_graph_t *graph,
    workflow_node_t **this_node,
    // Inputs
    const ExecutionSettings execution_settings);

StatusCode workflow_graph_find_input_node(
    // Outputs
    workflow_node_t **connected_node,
    //Inputs
    const workflow_graph_t *graph,
    const workflow_node_t *this_node,
    const char* port_name,
    const char* port_type
    );

#endif
