#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <vector>
#include <map>

struct vertex {
    int id;
};

template <class T>
struct vertexProperty 
{
    T value;
};

template <class T>
struct NoProperty 
{
    
};

template <class T>
struct edgeProperty 
{
    T value;

    // Overloading the operator!=
    bool operator!=(const edgeProperty<T> &other) const
    {
        return value != other.value;
    }
};

template <class vertex_property_type, class edge_property_type>
class Graph {
    private:
        int num_nodes; // number of nodes in the graph
        std::vector<std::map<int, float>> adjacencyList; // adjacency list (each vertex has a map of neighbors and edge weights)
        std::vector<vertex_property_type> vertexPropertiesMap; // map of vertex properties   (vertex id, vertex property)
        std::vector<std::map<int, edge_property_type>> edgePropertiesMap; // map of edge properties (edge id, edge property)

        // Add empty property values
        vertex_property_type emptyvertexProperty;
        edge_property_type emptyedgeProperty;
    public:
        Graph(int num_nodes) : num_nodes(num_nodes), emptyvertexProperty(), emptyedgeProperty() 
        {
            adjacencyList.resize(num_nodes);
            vertexPropertiesMap.resize(num_nodes);
            edgePropertiesMap.resize(num_nodes);
        }

        // addVertex
        void addVertex(vertex vertex)
        {
            adjacencyList[vertex.id] = std::map<int, float>();
            vertexPropertiesMap[vertex.id] = emptyvertexProperty;
        }

        // Methods for setting and getting vertex properties
        void setvertexProperty(int vertex, const vertex_property_type &property)
        {
            vertexPropertiesMap[vertex] = property;
        }

        void setvertexProperty(vertex vertex, const vertex_property_type &property)
        {
            setvertexProperty(vertex.id, property);
        }

        // getvertexProperty
        vertex_property_type getvertexProperty(int vertex) const
        {
            return vertexPropertiesMap[vertex];
        }

        vertex_property_type getvertexProperty(vertex vertex) const
        {
            return getvertexProperty(vertex.id);
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
            if (property != emptyedgeProperty) {
                edgePropertiesMap[source][target] = property;
            }else{
                //std::cout << "Edge property is empty" << std::endl;
            }
        }

        void setEdgeProperty(vertex source, vertex target, const edge_property_type &property)
        {
                edgePropertiesMap[source.id][target.id] = property;
        }

        void setEdgeProperty(vertex source, vertex target, const edge_property_type &property)
        {
            if (property != emptyedgeProperty) {
                edgePropertiesMap[source.id][target.id] = property;
            }else{
                //std::cout << "Edge property is empty" << std::endl;
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
