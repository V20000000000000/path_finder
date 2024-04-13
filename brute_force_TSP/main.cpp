#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Edge {
    int source;
    int destination;

    Edge(int src, int dest) : source(src), destination(dest) {} //constructor
};

void find_shortest_path(vector<int> nodes, int num_nodes, Edge edges[], int num_edges, int weights[], vector<int> &shortest_path, int &smallest_weight);

int main(void)
{
    int smallest_weight = 1000;
    vector <int> shortest_path;

    //initia;ize graph
    const int num_nodes = 9;
    vector<int> nodes = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    Edge edges[] = {Edge(0, 1), Edge(0, 3), Edge(1, 2), Edge(1, 4), Edge(2, 5), Edge(3, 4), Edge(4, 5), Edge(3, 6), Edge(6, 7), Edge(7, 8), Edge(5, 8), Edge(4, 7)};
    int weights[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    int num_edges = sizeof(edges) / sizeof(Edge);

    // find_shortest_path
    find_shortest_path(nodes, num_nodes, edges, num_edges, weights, shortest_path, smallest_weight);

    // Print the shortest path
    cout << "Shortest path: ";
    for (int node : shortest_path) {
        cout << node << " ";
    }
    cout << endl;

    cout << "Shortest weight: " << smallest_weight << endl;

    cout << "Complete!" << endl;

    return 0;
}

// find smallest path
void find_shortest_path(vector<int> nodes, int num_nodes, Edge edges[], int num_edges, int weights[], vector<int> &shortest_path, int &smallest_weight)
{
    vector <int> temp_path;

    //find all legal paths
    do {
        temp_path.clear();  //clear the temp path

        //copy the nodes to the temp path
        for (int node : nodes) {
            temp_path.push_back(node);
        }

        //check if path is legal
        bool edge_exists = true;
        for (int i = 0; i < num_edges; i++) {
            if(!(edges[i].source == temp_path[i] && edges[i].destination == temp_path[i + 1])) {
                edge_exists = false;
                break;
            }
        }

        //if path is legal, calculate the weight    
        if (edge_exists) {
            int weight = 0;
            for (int i = 0; i < num_edges; i++) {
                weight += weights[i];
            }

            //if the weight is less than the current shortest path, update the shortest path
            if (weight < smallest_weight) {
                shortest_path.clear();
                for (int i = 0; i < num_nodes; i++) {
                    shortest_path.push_back(temp_path[i]);
                }
                smallest_weight = weight;
            }
        }
    } while (std::next_permutation(nodes.begin(), nodes.end()));
}

