#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <random>

#include "Graph.hpp"
#include "FindShortestPath.hpp"

using namespace std;

int smallest_weight = numeric_limits<int>::max();
int num_nodes = 6;

int main(void)
{
    vector <int> shortest_path;

    //initialize graph
    Graph graph(num_nodes);

    //set weights
    random_device rd;  
    mt19937 gen(rd()); 
    uniform_int_distribution<> dis(1, 20);

    for(int i = 0; i < num_nodes; i++)
    {
        for(int j = 0; j < num_nodes; j++)
        {
            if(i == j)
            {
                graph.setWeight(i, j, 0);
            }
            else
            {
                graph.setWeight(i, j, dis(gen));
            }
        }
    }

    //print weights
    for(int i = 0; i < num_nodes; i++)
    {
        for(int j = 0; j < num_nodes; j++)
        {
            cout << "node " << i << " to node " << j << ": " << graph.getWeight(i, j) << endl;
        }
        cout << endl;
    }

    //find smallest path
    FindShortestPath find_shortest_path(num_nodes);
    find_shortest_path.find_shortest_path(graph);

    //print smallest path
    shortest_path = find_shortest_path.get_shortest_path();
    cout << "Shortest path: ";
    for(int i = 0; i < num_nodes; i++)
    {
        cout << shortest_path[i] << " ";
    }
    cout << endl;

    //print smallest weight
    cout << "Smallest weight: " << find_shortest_path.get_smallest_weight() << endl;

    cout << "Complete!" << endl;
    return 0;
}



