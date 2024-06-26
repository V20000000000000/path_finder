#include <iostream>
#include "Graph.hpp"

using namespace std;



int main()
{
    Graph<vertexProperty<int>, edgeProperty<int>> graph(5);

    vertex v1{0}, v2{1}, v3{2}, v4{3}, v5{4};

    graph.addVertex(v1);
    graph.addVertex(v2);
    graph.addVertex(v3);
    graph.addVertex(v4);
    graph.addVertex(v5);

    graph.addDirectedEdge(v1, v2, 1);
    graph.addDirectedEdge(v1, v3, 2);
    graph.addDirectedEdge(v2, v3, 3);
    graph.addDirectedEdge(v3, v4, 4);
    graph.addDirectedEdge(v4, v5, 5);

    // Set vertex property
    graph.setvertexProperty(0, vertexProperty<int>{10});
    graph.setvertexProperty(1, vertexProperty<int>{20});
    graph.setvertexProperty(2, vertexProperty<int>{30});
    graph.setvertexProperty(3, vertexProperty<int>{40});
    graph.setvertexProperty(4, vertexProperty<int>{50});

    // Get vertex property
    vertexProperty<int> vp = graph.getvertexProperty(0);
    cout << "Vertex property of vertex 0: " << vp.value << endl;

    // Get edge property
    edgeProperty<int> ep = graph.getEdgeProperty(0, 1);
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
    float weight = graph.getEdgeWeight(0, 1);
    cout << "Weight of edge (0, 1): " << weight << endl;

    return 0;
    
}