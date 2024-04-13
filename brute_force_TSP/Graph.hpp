#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>

class Graph {
    private:
        int num_nodes_;
        std::vector<std::vector<float>> weights_;
    public:
        Graph(int num_nodes) : num_nodes_(num_nodes), weights_(num_nodes, std::vector<float>(num_nodes, 1.0)) {}

        //vertex number
        int size() const {
            return num_nodes_;
        }

        // get the weight of the edge between two nodes
        float getWeight(int source, int destination) const {
            return weights_[source][destination];
        }

        // set the weight of the edge between two nodes 
        void setWeight(int source, int destination, float weight) {
            weights_[source][destination] = weight;
            weights_[destination][source] = weight; 
        }
};

#endif // GRAPH_HPP
