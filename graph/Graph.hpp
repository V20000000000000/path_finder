#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <vector>
#include <map>

struct Vertex {
    int Vertex_id;
};

template <class vertex_property_type, class edge_property_type>
class Graph {
    private:
        int num_nodes;
        int max_id;
        std::vector<std::map<int, float>> adjacencyList; // adjacency list (each vertex has a map of neighbors and edge weights)
        std::vector<vertex_property_type> vertexProperties_map; // map of vertex properties   (vertex id, vertex property)
        std::vector<std::map<int, edge_property_type>> edgeProperties_map; // map of edge properties (edge id, edge property)
    public:
        Graph(int num_nodes): num_nodes(num_nodes), max_id(0){
            adjacencyList.resize(num_nodes);
            vertexProperties_map.resize(num_nodes);
            edgeProperties_map.resize(num_nodes);
        }

        // addVertex (vertex)
        void addVertex(Vertex &vertex) {
            // Set the new vertex ID to be one more than the maximum
            vertex.Vertex_id = max_id;

            std::cout << "Vertex ID: " << vertex.Vertex_id << std::endl;

            // Set the new vertex and its properties directly at the specified index
            adjacencyList[vertex.Vertex_id] = std::map<int, float>();
            vertexProperties_map[vertex.Vertex_id] = vertex_property_type(vertex.Vertex_id);
            max_id++;
        }

        // addEdge   (source, target, weight)
        void addEdge(Vertex source, Vertex target, float weight) {
            // Check if source and target vertices exist
            if (source.Vertex_id >= num_nodes || target.Vertex_id >= num_nodes) {
                // One or both of the vertices do not exist in the graph
                std::cerr << "Error: One or both of the vertices do not exist in the graph." << std::endl;
                return;
            }

            // Update adjacency list
            adjacencyList[source.Vertex_id][target.Vertex_id] = weight;
            adjacencyList[target.Vertex_id][source.Vertex_id] = weight;

            // Update edge properties map
            edgeProperties_map[source.Vertex_id][target.Vertex_id] = weight;
            edgeProperties_map[target.Vertex_id][source.Vertex_id] = weight;
        }

        // getNeighbors
        std::vector<int> getNeighbors(Vertex vertex) {
            std::vector<int> neighbors;
            for (auto neighbor : adjacencyList[vertex.Vertex_id]) {
                neighbors.push_back(neighbor.first);
            }
            return neighbors;
        }

        // getEdgeWeight
        float getEdgeWeight(Vertex source, Vertex target) {
            if (adjacencyList[source.Vertex_id].find(target.Vertex_id) != adjacencyList[source.Vertex_id].end()) {
                return adjacencyList[source.Vertex_id][target.Vertex_id];
            } else {
                return -1.0f; // Indicate edge doesn't exist
            }
        }

        // size (adjacency list size)
        int size() {
            return adjacencyList.size();
        }
};

#endif // GRAPH_HPP
