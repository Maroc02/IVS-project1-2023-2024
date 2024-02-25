//======= Copyright (c) 2024, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - graph
//
// $NoKeywords: $ivs_project_1 $tdd_code.cpp
// $Author:     Marek Čupr <xcuprm01@stud.fit.vutbr.cz>
// $Date:       $2024-02-15
//============================================================================//
/**
 * @file tdd_code.cpp
 * @author Martin Dočekal
 * @author Karel Ondřej
 *
 * @brief Implementace metod tridy reprezentujici graf.
 */

#include "tdd_code.h"

Graph::Graph(){
    // Initialize empty graph
    this->graph_nodes = {};
    this->graph_edges = {};
}

Graph::~Graph(){
    // Clear all nodes and edges from the graph
    this->clear();
}

std::vector<Node*> Graph::nodes() {
    // Return vector of all graph nodes
    return this->graph_nodes;
}

std::vector<Edge> Graph::edges() const{
    // Return vector of all graph edges
    return this->graph_edges;
}

Node* Graph::addNode(size_t nodeId) {
    // Check if the node exists
    for (Node* node_i : this->graph_nodes){
        if (node_i->id == nodeId) 
            return nullptr;
    }

    // Create new node
    Node* new_node = new Node();
    new_node->id = nodeId;
    new_node->color = 0;
    new_node->degree = 0;

    // Add node to the graph
    this->graph_nodes.push_back(new_node);
    return new_node;
}

bool Graph::addEdge(const Edge& edge){
    // Check if edge is a loop or duplicit 
    if (edge.a == edge.b || containsEdge(edge))
        return false;

    // Add edge to the graph
    this->graph_edges.push_back(edge);
    // Check if edge nodes exist
    addNode(edge.a);
    addNode(edge.b);

    return true;
}

void Graph::addMultipleEdges(const std::vector<Edge>& edges) {
    // Loop through all edges and add them to the graph
    for (Edge edge_i : edges)
        addEdge(edge_i);
}

Node* Graph::getNode(size_t nodeId){
    // Search for node in the graph
    for (Node* node_i : this->graph_nodes){
        if (node_i->id == nodeId)
            return node_i;
    }

    return nullptr;
}

bool Graph::containsEdge(const Edge& edge) const{
    // Check if edge exists
    for (Edge edge_i : this->graph_edges){
        if ((edge_i.a == edge.a && edge_i.b == edge.b) || (edge_i.a == edge.b && edge_i.b == edge.a))
            return true;
    }

    return false;
}

void Graph::removeNode(size_t nodeId){
    // Search for node in the graph
    for (int i = this->graph_nodes.size() - 1; i >= 0; i--){
        if (this->graph_nodes[i]->id == nodeId){
            // Search edges connected to the node
            for (int j = this->graph_edges.size() - 1; j >= 0; j--){
                if (this->graph_edges[j].a == this->graph_nodes[i]->id || this->graph_edges[j].b == this->graph_nodes[i]->id ){
                    // Remove edges connected to the node
                    this->graph_edges.erase(this->graph_edges.begin() + j);
                }
            }

            // Remove the node
            delete this->graph_nodes[i];
            this->graph_nodes.erase(this->graph_nodes.begin() + i);
            return;
        }
    }

    // The node doesn't exist
    throw std::out_of_range("Error at function removeNode: Attempting to delete a non-existent node!\n");
}

void Graph::removeEdge(const Edge& edge){
    // Search for edge in the graph
    for (int i = this->graph_edges.size() - 1; i >= 0; i--){
        if ((this->graph_edges[i].a == edge.a && this->graph_edges[i].b == edge.b) || (this->graph_edges[i].a == edge.b && this->graph_edges[i].b == edge.a)){
            // Remove the edge
            this->graph_edges.erase(this->graph_edges.begin() + i );
            return;
        }
    }

    // The edge doesn't exist
    throw std::out_of_range("Error at function removeEdge: Attempting to delete a non-existent edge!\n");
}

size_t Graph::nodeCount() const{
    // Return nodes count
    return this->graph_nodes.size();
}

size_t Graph::edgeCount() const{
    // Return edges count
    return this->graph_edges.size();
}

size_t Graph::nodeDegree(size_t nodeId) const{
    // Default value
    size_t count = 0;
    
    // Check node degree
    for (Edge edge : this->graph_edges){
        if (edge.a == nodeId || edge.b == nodeId)
            count++;
    }

    // The node doesn't exist
    if (!count)
        throw std::out_of_range("Error at function nodeDegree: Attempting to count degree of non-existent node!\n");

    return count;
}

size_t Graph::graphDegree() const {
    // Default value
    size_t max_degree = 0;

    // Find max graph degree
    for (Node* node_i : this->graph_nodes)
        max_degree = std::max(max_degree, nodeDegree(node_i->id));

    return max_degree;
}

void Graph::coloring(){
    // Get max colors count
    size_t colors_max = graphDegree() + 1;
    // Starting coloring value
    size_t current_color = 1;

    // Loop through all nodes and color them
    for (Node* node : this->graph_nodes){
        node->color = current_color;

        // Reset coloring value to 1 if needed
        if (current_color == colors_max)
            current_color = 1;

        // Move to next color
        current_color++;
    }
}
void Graph::clear() {
    // Dynamically free the graph nodes
    for (Node* node : this->graph_nodes)
        delete node;

    // Delete all vectors
    this->graph_nodes.clear();
    this->graph_edges.clear();
}

/*** Konec souboru tdd_code.cpp ***/
