#include <map>
#include <vector>
#include <iostream>

using namespace std;

struct Vertex
{
    int id;
};

// Define a generic class for vertex property
template <class T>
struct VertexProperty
{
    T value;
    // You can add more properties here as needed
    // overloading the operator!=
    bool operator!=(const VertexProperty<T> &other) const
    {
        return value != other.value;
    }
};

// Define a generic class for edge property
template <class T>
struct EdgeProperty
{
    T value;
    // You can add more properties here as needed
    // overloading the operator!=
    bool operator!=(const EdgeProperty<T> &other) const
    {
        return value != other.value;
    }
};

template <class vertex_property_type, class edge_property_type>
class Graph
{
private:
    vector<map<int, float>> adjacencyList;
    vector<vertex_property_type> vertex_property_map;
    vector<map<int, edge_property_type>> edge_property_map;

    // Add empty property values
    vertex_property_type empty_vertex_property = vertex_property_type();
    edge_property_type empty_edge_property = edge_property_type();

public:
    Graph(int n) : adjacencyList(n), vertex_property_map(n), edge_property_map(n) {}

    void addDirectedEdge(int source, int target, float weight, vertex_property_type vertex_property, edge_property_type edge_property)
    {
        adjacencyList[source][target] = weight;
        if (vertex_property != empty_vertex_property)
            vertex_property_map[source] = vertex_property;
        if (edge_property != empty_edge_property)
            edge_property_map[source][target] = edge_property;
    }

    void addDirectedEdge(int source, int target, float weight)
    {
        addDirectedEdge(source, target, weight, empty_vertex_property, empty_edge_property);
    }

    void addDirectedEdge(Vertex &source, Vertex &target, float weight)
    {
        addDirectedEdge(source.id, target.id, weight);
    }

    void addBidirectedEdge(int source, int target, float weight1, float weight2)
    {
        addDirectedEdge(source, target, weight1);
        addDirectedEdge(target, source, weight2);
    }

    void addBidirectedEdge(Vertex &source, Vertex &target, float weight1, float weight2)
    {
        addBidirectedEdge(source.id, target.id, weight1, weight2);
    }

    vector<int> getNeighbors(int vertex) const
    {
        vector<int> neighbors;
        for (const auto &neighbor : adjacencyList[vertex])
        {
            neighbors.push_back(neighbor.first);
        }
        return neighbors;
    }

    vector<int> getNeighbors(Vertex &vertex) const
    {
        return getNeighbors(vertex.id);
    }

    float getWeight(int source, int target) const
    {
        return adjacencyList[source].at(target);
    }

    float getWeight(Vertex &source, Vertex &target) const
    {
        return getWeight(source.id, target.id);
    }

    int size() const
    {
        return adjacencyList.size();
    }

    const map<int, float> &operator[](int vertex) const
    {
        return adjacencyList[vertex];
    }

    // Methods for setting and getting vertex properties
    void setVertexProperty(int vertex, const vertex_property_type &property)
    {
        vertex_property_map[vertex] = property;
    }

    vertex_property_type getVertexProperty(int vertex) const
    {
        return vertex_property_map[vertex];
    }

    // Methods for setting and getting edge properties
    void setEdgeProperty(int source, int target, const edge_property_type &property)
    {
        edge_property_map[source][target] = property;
    }

    edge_property_type getEdgeProperty(int source, int target) const
    {
        return edge_property_map[source].at(target);
    }
};

int main()
{
    Graph<VertexProperty<int>, EdgeProperty<int>> graph(5);

    Vertex v1{0}, v2{1}, v3{2}, v4{3}, v5{4};

    graph.addDirectedEdge(v1, v2, 1);
    graph.addDirectedEdge(v1, v3, 2);
    graph.addDirectedEdge(v2, v3, 3);
    graph.addDirectedEdge(v3, v4, 4);
    graph.addDirectedEdge(v4, v5, 5);

    // Set vertex property
    graph.setVertexProperty(0, VertexProperty<int>{10});
    graph.setVertexProperty(1, VertexProperty<int>{20});
    graph.setVertexProperty(2, VertexProperty<int>{30});
    graph.setVertexProperty(3, VertexProperty<int>{40});
    graph.setVertexProperty(4, VertexProperty<int>{50});

    // Set edge property
    graph.setEdgeProperty(0, 1, EdgeProperty<int>{100});
    graph.setEdgeProperty(0, 2, EdgeProperty<int>{200});
    graph.setEdgeProperty(1, 2, EdgeProperty<int>{300});
    graph.setEdgeProperty(2, 3, EdgeProperty<int>{400});
    graph.setEdgeProperty(3, 4, EdgeProperty<int>{500});

    // Get vertex property
    VertexProperty<int> vp = graph.getVertexProperty(0);
    cout << "Vertex property of vertex 0: " << vp.value << endl;

    // Get edge property
    EdgeProperty<int> ep = graph.getEdgeProperty(0, 1);
    cout << "Edge property of edge (0, 1): " << ep.value << endl;

    // Get neighbors of a vertex
    vector<int> neighbors = graph.getNeighbors(0);
    cout << "Neighbors of vertex 0: ";
    for (int neighbor : neighbors)
    {
        cout << neighbor << " ";
    }
    cout << endl;

    // Get weight of an edge
    float weight = graph.getWeight(0, 1);
    cout << "Weight of edge (0, 1): " << weight << endl;

    return 0;
}