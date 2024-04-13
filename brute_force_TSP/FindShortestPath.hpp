#ifndef FINDSHORTESTPATH_HPP
#define FINDSHORTESTPATH_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include "Graph.hpp"

using namespace std;

class FindShortestPath
{
    private:
        int num_nodes;
        int smallest_weight;
        vector <int> nodes;
        vector <int> shortest_path;
    public:
        //constructor
        FindShortestPath(int num_nodes) : num_nodes(num_nodes), smallest_weight(numeric_limits<int>::max())
        {
            for(int i = 0; i < num_nodes; i++)
            {
                nodes.push_back(i);
            }
        }
        
        // find smallest path
        void find_shortest_path(Graph& graph)
        {
            do {
                int weight = 0;
                for(int i = 0; i < num_nodes - 1; i++)
                {
                    weight += graph.getWeight(nodes[i], nodes[i + 1]);
                }
                if(weight < smallest_weight)
                {
                    smallest_weight = weight;
                    shortest_path = nodes;
                }
            } 
            while 
            (std::next_permutation(nodes.begin(), nodes.end()));
        }

        //print smallest path
        vector<int> get_shortest_path()
        {
            return shortest_path;
        }

        //get smallest weight
        int get_smallest_weight()
        {
            return smallest_weight;
        }
};

#endif 
