#include <iostream>
#include "Graph.hpp"

using namespace std;



int main()
{
    Graph<int, int> g(5);

    vertex v1{1}, v2{2}, v3{3}, v4{4}, v5{5};

    cout << "Size of the adjacency list: " << g.size() << endl;

    //addVertex
    g.addVertex(v1);
    g.addVertex(v2);
    g.addVertex(v3);
    g.addVertex(v4);
    g.addVertex(v5);

    cout << "test" << endl;

    //addEdge
    g.addBidirectedEdge(v1, v2, 1, 1);
    g.addBidirectedEdge(v1, v3, 7, 7);
    g.addBidirectedEdge(v1, v4, 1, 5);
    g.addDirectedEdge(v1, v5, 3);
    g.addDirectedEdge(v2, v3, 1);

    //getNeighbors
    vector<int> neighbors = g.getNeighbors(v1);
    cout << "Neighbors of vertex 1: ";
    for (unsigned i = 0; i < neighbors.size(); i++)
    {
        cout << neighbors[i] << " ";
    }
    cout << endl;

    neighbors = g.getNeighbors(v2);
    cout << "Neighbors of vertex 2: ";
    for (unsigned i = 0; i < neighbors.size(); i++)
    {
        cout << neighbors[i] << " ";
    }
    cout << endl;

    neighbors = g.getNeighbors(v3);
    cout << "Neighbors of vertex 3: ";
    for (unsigned i = 0; i < neighbors.size(); i++)
    {
        cout << neighbors[i] << " ";
    }
    cout << endl;

    neighbors = g.getNeighbors(v4);
    cout << "Neighbors of vertex 4: ";
    for (unsigned i = 0; i < neighbors.size(); i++)
    {
        cout << neighbors[i] << " ";
    }
    cout << endl;

    neighbors = g.getNeighbors(v5);
    cout << "Neighbors of vertex 5: ";
    for (unsigned i = 0; i < neighbors.size(); i++)
    {
        cout << neighbors[i] << " ";
    }
    cout << endl;

    //getEdgeWeight
    cout << "Edge weight between vertex 1 and vertex 2: " << g.getEdgeWeight(v1, v2) << endl;
    cout << "Edge weight between vertex 1 and vertex 3: " << g.getEdgeWeight(v1, v3) << endl;
    cout << "Edge weight between vertex 1 and vertex 4: " << g.getEdgeWeight(v1, v4) << endl;
    cout << "Edge weight between vertex 1 and vertex 5: " << g.getEdgeWeight(v1, v5) << endl;
    cout << "Edge weight between vertex 2 and vertex 3: " << g.getEdgeWeight(v2, v3) << endl;
    cout << "Edge weight between vertex 2 and vertex 4: " << g.getEdgeWeight(v2, v4) << endl;
    cout << "Edge weight between vertex 2 and vertex 5: " << g.getEdgeWeight(v2, v5) << endl;
    cout << "Edge weight between vertex 5 and vertex 2: " << g.getEdgeWeight(v5, v2) << endl;
    cout << "Edge weight between vertex 1 and vertex 1: " << g.getEdgeWeight(v1, v1) << endl;

    //size(adjacency list size)
    cout << "Size of the adjacency list: " << g.size() << endl;
    
}