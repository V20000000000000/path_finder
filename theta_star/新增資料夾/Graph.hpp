#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <unordered_map>
#include <set>
#include <queue>
#include <tuple>
#include <algorithm>
#include <cassert>

using namespace std;

// Define a structure for vertex properties
template <class T>
struct VertexProperty
{
    T value;
    VertexProperty(T val) : value(val) {}
    VertexProperty() {} // 預設建構子
};

// Define a structure for edge properties
template <class U>
struct EdgeProperty
{
    U value;
    EdgeProperty() {} // 預設建構子
    EdgeProperty(U val) : value(val) {}

    // Overloading the operator!=
    bool notEquals(const EdgeProperty<U> &other) const
    {
        return value != other.value;
    }
};

// Define a structure for vertices
struct Vertex
{
    int id;
    Vertex(int i) : id(i) {}
    int getId() const { return id; }
};

// Define a class for the graph
template <class T, class U>
class Graph
{
private:
    std::vector<std::map<int, float>> adjacencyList;
    std::vector<std::unordered_map<int, EdgeProperty<U>>> edgePropertiesMap;
    EdgeProperty<U> emptyEdgeProperty = EdgeProperty<U>();
    VertexProperty<T> emptyVertexProperty = VertexProperty<T>();
    std::vector<VertexProperty<T>> vertexPropertiesMap; // set every vertex's property(width)

public:
    // Constructor
    Graph() {}

    Graph(int numNodes)
        : adjacencyList(numNodes),
          edgePropertiesMap(numNodes)
    {
        cout << "start init Graph" << endl;
        for (int i = 0; i < numNodes; ++i)
        {
            vertexPropertiesMap.push_back(emptyVertexProperty);
        }
        cout << "finish init Graph" << endl;
    }

    // Method to add a bidirected edge
    void addBidirectedEdge(int source, int target, double weight1, double weight2)
    {
        addDirectedEdge(source, target, weight1);
        addDirectedEdge(target, source, weight2);
    }

    void addBidirectedEdge(const Vertex &source, const Vertex &target, double weight1, double weight2)
    {
        addBidirectedEdge(source.getId(), target.getId(), weight1, weight2);
    }

    // Method to add an undirected edge
    void addUndirectedEdge(int source, int target, double weight) 
    {
        addBidirectedEdge(source, target, weight, weight);
    }

    void addUndirectedEdge(const Vertex &source, const Vertex &target, double weight)
    {
        addUndirectedEdge(source.getId(), target.getId(), weight);
    }

    // Method to add a directed edge
    void addDirectedEdge(int source, int target, double weight)
    {
        adjacencyList[source][target] = weight;
    }

    void addDirectedEdge(const Vertex &source, const Vertex &target, double weight)
    {
        addDirectedEdge(source.getId(), target.getId(), weight);
    }

    // Method to get edge property
    EdgeProperty<U> getEdgeProperty(int source, int target) const
    {
        auto it = edgePropertiesMap[source].find(target);
        if (it != edgePropertiesMap[source].end())
        {
            return it->second;
        }
        return emptyEdgeProperty;
    }

    EdgeProperty<U> getEdgeProperty(const Vertex &source, const Vertex &target) const
    {
        return getEdgeProperty(source.getId(), target.getId());
    }

    // Method to get edge weight
    double getEdgeWeight(int source, int target) const
    {
        auto it = adjacencyList[source].find(target);
        if (it != adjacencyList[source].end())
        {
            return it->second;
        }
        return -1.0f;
    }

    double getEdgeWeight(const Vertex &source, const Vertex &target) const
    {
        return getEdgeWeight(source.getId(), target.getId());
    }

    // Method to get neighbors of a vertex
    std::vector<int> getNeighbors(int vertex) const
    {
        std::vector<int> neighbors;
        for (const auto &neighbor : adjacencyList[vertex])
        {
            neighbors.push_back(neighbor.first);
        }
        return neighbors;
    }

    std::vector<int> getNeighbors(const Vertex &vertex) const
    {
        return getNeighbors(vertex.getId());
    }

    // Method to get vertex property
    VertexProperty<T> getVertexProperty(int vertex) const
    {
        return vertexPropertiesMap[vertex];
    }

    VertexProperty<T> getVertexProperty(const Vertex &vertex) const
    {
        return getVertexProperty(vertex.getId());
    }

    // Method to set edge property
    void setEdgeProperty(int source, int target, const EdgeProperty<U> &property)
    {
        if (property.notEquals(emptyEdgeProperty))
        {
            edgePropertiesMap[source][target] = property;
        }
    }

    void setEdgeProperty(const Vertex &source, const Vertex &target, const EdgeProperty<U> &property)
    {
        setEdgeProperty(source.getId(), target.getId(), property);
    }

    // Method to set edge weight
    void setEdgeWeight(int source, int target, double weight)
    {
        adjacencyList[source][target] = weight;
    }

    void setEdgeWeight(const Vertex &source, const Vertex &target,double weight)
    {
        setEdgeWeight(source.getId(), target.getId(), weight);
    }

    // Method to set vertex property
    void setVertexProperty(int vertex, const VertexProperty<T> &property)
    {
        vertexPropertiesMap[vertex] = property;
    }

    void setVertexProperty(const Vertex &vertex, const VertexProperty<T> &property)
    {
        setVertexProperty(vertex.getId(), property);
    }

    // Method to get size
    int size() const
    {
        return adjacencyList.size();
    }
};

#endif // GRAPH_HPP
