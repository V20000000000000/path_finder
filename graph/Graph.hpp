#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <vector>
#include <map>

struct vertex {
    int id;
};

template <class vertex_property_type, class edge_property_type>
class Graph {
    private:
        int num_nodes; // number of nodes in the graph
        std::vector<std::map<int, float>> adjacencyList; // adjacency list (each vertex has a map of neighbors and edge weights)
        std::vector<vertex_property_type> vertexPropertiesMap; // map of vertex properties   (vertex id, vertex property)
        std::vector<std::map<int, edge_property_type>> edgePropertiesMap; // map of edge properties (edge id, edge property)

        // Add empty property values
        vertex_property_type emptyVertexProperty;
        edge_property_type emptyEdgeProperty;
    public:
        Graph(int num_nodes) : num_nodes(num_nodes), emptyVertexProperty(), emptyEdgeProperty() 
        {
            adjacencyList.resize(num_nodes);
            vertexPropertiesMap.resize(num_nodes);
            edgePropertiesMap.resize(num_nodes);
        }

        // addVertex
        void addVertex(vertex vertex)
        {
            adjacencyList[vertex.id] = std::map<int, float>();
            vertexPropertiesMap[vertex.id] = emptyVertexProperty;
        }

        // Methods for setting and getting vertex properties
        void setVertexProperty(int vertex, const vertex_property_type &property)
        {
            vertexPropertiesMap[vertex] = property;
        }

        void setVertexProperty(vertex vertex, const vertex_property_type &property)
        {
            setVertexProperty(vertex.id, property);
        }

        // getVertexProperty
        vertex_property_type getVertexProperty(int vertex) const
        {
            return vertexPropertiesMap[vertex];
        }

        vertex_property_type getVertexProperty(vertex vertex) const
        {
            return getVertexProperty(vertex.id);
        }

        // addEdge (source, target, weight)
        void addDirectedEdge(int source, int target, float weight)
        {
            adjacencyList[source][target] = weight;
        }

        void addDirectedEdge(vertex &source, vertex &target, float weight)
        {
            addDirectedEdge(source.id, target.id, weight);
        }

        void addBidirectedEdge(int source, int target, float weight1, float weight2)
        {
            addDirectedEdge(source, target, weight1);
            addDirectedEdge(target, source, weight2);
        }

        void addBidirectedEdge(vertex &source, vertex &target, float weight1, float weight2)
        {
            addBidirectedEdge(source.id, target.id, weight1, weight2);
        }

        // Methods for Edge setting
        void setEdgeProperty(int source, int target, const edge_property_type &property)
        {
            if (property != emptyEdgeProperty) {
                edgePropertiesMap[source][target] = property;
            }else{
                std::cout << "Edge property is empty" << std::endl;
            }
        }

        edge_property_type getEdgeProperty(int source, int target) const
        {
            return edgePropertiesMap[source].at(target);
        }

        // getNeighbors
        std::vector<int> getNeighbors(int vertex)  const
        {
            std::vector<int> neighbors;
            for (auto neighbor : adjacencyList[vertex]) {
                neighbors.push_back(neighbor.first);
            }
            return neighbors;
        }

        std::vector<int> getNeighbors(vertex vertex)  const
        {
            return getNeighbors(vertex.id);
        }

        // getEdgeWeight
        float getEdgeWeight(int source, int target) 
        {
            if (adjacencyList[source].find(target) != adjacencyList[source].end()) {
                return adjacencyList[source].at(target);
            } else {
                return -1.0f; // Indicate edge doesn't exist
            }
        }

        float getEdgeWeight(vertex source, vertex target) 
        {
            return getEdgeWeight(source.id, target.id);
        }

        // size (adjacency list size)
        int size() 
        {
            return adjacencyList.size();
        }
};

#endif // GRAPH_HPP
