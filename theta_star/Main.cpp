#include <iostream>
#include <vector>
#include <stack>
#include <unordered_set>
#include <queue>
#include <cmath>
#include <limits>

#include "Graph.hpp"
#include "Block.hpp"

// Pair represents a pair of values
template<typename T, typename U>
struct Pair {
    T first;
    U second;
    Pair(T first, U second) : first(first), second(second) {}
};

// HeuristicInterface represents the interface for heuristic functions
class HeuristicInterface {
public:
    virtual float get(const Graph<Block, int>& graph, const Vertex& source, const Vertex& target) const = 0;
};

// HeuristicA is an example heuristic function
class HeuristicA : public HeuristicInterface {
public:
    float get(const Graph<Block, int>& graph, const Vertex& source, const Vertex& target) const override {
        int dx = target.getId() - source.getId();
        return std::sqrt(dx * dx);
    }
};

// AStar represents the A* algorithm implementation
class AStar {
public:
    static std::stack<Vertex> reconstructPath(const Vertex& source, const Vertex& target, const std::vector<int>& pred) {
        std::stack<Vertex> path;
        int current = target.getId();
        while (current != -1 && current != source.getId()) {
            path.push(Vertex(current));
            current = pred[current];
        }
        path.push(Vertex(source.getId()));
        return path;
    }

    static std::stack<Vertex> run(const Vertex& source, const Vertex& target, const Graph<Block, int>& graph,
                                  const HeuristicInterface& heuristic) {
        std::vector<int> dist(graph.size(), std::numeric_limits<int>::max());
        std::vector<int> pred(graph.size(), -1);
        std::unordered_set<int> closedSet;
        dist[source.getId()] = 0;

        auto cmp = [](const Pair<float, int>& left, const Pair<float, int>& right) {
            return left.first > right.first;
        };
        std::priority_queue<Pair<float, int>, std::vector<Pair<float, int>>, decltype(cmp)> open(cmp);
        open.push({heuristic.get(graph, source, target), source.getId()});  //將(距離，起點)放入openqueue

        while (!open.empty()) {
            int currentVertex = open.top().second;  //取出openqueue中的最小值
            open.pop(); //將最小值pop出來

            if (currentVertex == target.getId()) {  //如果最小值等於終點，則回傳路徑
                return reconstructPath(source, target, pred);
            }

            closedSet.insert(currentVertex);    //將最小值放入closedset

            for (const auto& neighbor : graph.getNeighbors(currentVertex)) {  
                int edgeWeight = graph.getEdgeWeight(currentVertex, neighbor);   //取出邊的權重

                if (closedSet.find(neighbor) != closedSet.end()) continue;  //如果鄰居在closedset中，則跳過

                int alt = dist[currentVertex] + edgeWeight; //計算新的距離
                if (alt < dist[neighbor]) { //如果新的距離小於原本的距離
                    dist[neighbor] = alt;   //更新距離
                    pred[neighbor] = currentVertex; //更新前驅
                    open.push({dist[neighbor] + heuristic.get(graph, source, target), neighbor});   //將(距離，鄰居)放入openqueue
                }
            }
        }

        return std::stack<Vertex>();    //回傳空的stack
    }
};

int main() {
    // Create graph
    Graph<Block, int> graph(11);

    // Create vertices
    Vertex v1(0), v2(1), v3(2), v4(3), v5(4), v6(5), v7(6), v8(7), v9(8), v10(9);
    // Add undirected edges
    graph.addUndirectedEdge(v5, v6, 3);
    graph.addUndirectedEdge(v5, v4, 3);
    graph.addUndirectedEdge(v5, v3, 3);
    graph.addUndirectedEdge(v5, v1, 3);
    graph.addUndirectedEdge(v5, v7, 4);
    graph.addUndirectedEdge(v2, v8, 5);
    graph.addUndirectedEdge(v9, v5, 3);
    graph.addUndirectedEdge(v6, v4, 1);
    graph.addUndirectedEdge(v4, v3, 1);
    graph.addUndirectedEdge(v3, v1, 1);
    graph.addUndirectedEdge(v1, v7, 1);
    graph.addUndirectedEdge(v6, v10, 4);
    graph.addUndirectedEdge(v4, v10, 3);
    graph.addUndirectedEdge(v3, v10, 2);
    graph.addUndirectedEdge(v1, v10, 1);
    graph.addUndirectedEdge(v7, v10, 1);
    graph.addUndirectedEdge(v10, v2, 2);


    // Create heuristic function
    HeuristicA heuristic;

    // Create source and target vertices
    Vertex source = v9;
    Vertex target = v8;

    // Run Theta* algorithm
    std::stack<Vertex> path = AStar::run(source, target, graph, heuristic);

    // Print path
    while (!path.empty()) {
        std::cout << path.top().getId() << "->";
        path.pop();
    }
    std::cout << std::endl;

    return 0;
}

