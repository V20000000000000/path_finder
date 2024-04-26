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

// obstacle
struct Obstacle
{
    double minX;
    double minY;
    double minZ;
    double maxX;
    double maxY;
    double maxZ;
    Obstacle(double minX, double minY, double minZ, double maxX, double maxY, double maxZ) : minX(minX), minY(minY), minZ(minZ), maxX(maxX), maxY(maxY), maxZ(maxZ) {}
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
        double dx = graph.getVertexProperty(target).value.getX() - graph.getVertexProperty(source).value.getX();
        double dy = graph.getVertexProperty(target).value.getY() - graph.getVertexProperty(source).value.getY();
        double dz = graph.getVertexProperty(target).value.getZ() - graph.getVertexProperty(source).value.getZ();
        return std::sqrt(dx * dx + dy * dy + dz * dz);
    }
};

// AStar represents the A* algorithm implementation
class ThetaStar {
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

    static bool lineOfSight(const Vertex& source, const Vertex& target, const Graph<Block, int>& graph, const std::vector<Obstacle>& obstacles)
    {
        double x0 = graph.getVertexProperty(source).value.getX();
        double y0 = graph.getVertexProperty(source).value.getY();
        double z0 = graph.getVertexProperty(source).value.getZ();
        double x1 = graph.getVertexProperty(target).value.getX();
        double y1 = graph.getVertexProperty(target).value.getY();
        double z1 = graph.getVertexProperty(target).value.getZ();
        double dx = x1 - x0;
        double dy = y1 - y0;
        double dz = z1 - z0;
        for (const auto& obstacle : obstacles) {
            for(double step = 0; step < 1; step += 0.001)
            {
                double x = x0 + dx * step;
                double y = y0 + dy * step;
                double z = z0 + dz * step;
                if (x >= obstacle.minX && x <= obstacle.maxX && y >= obstacle.minY && y <= obstacle.maxY && z >= obstacle.minZ && z <= obstacle.maxZ) {
                    return false;
                }
            }
        }
        return true;
    }

    static std::stack<Vertex> run(const Vertex& source, const Vertex& target, const Graph<Block, int>& graph,
                                  const HeuristicInterface& heuristic, const std::vector<Obstacle>& obstacles) {
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

            int pp = pred[currentVertex];  //取出前前驅
            

            for (const auto& neighbor : graph.getNeighbors(currentVertex)) {

                if(lineOfSight(source, target, graph, obstacles))
                {

                    int edgeWeight1 = graph.getEdgeWeight(currentVertex, neighbor);   //取出邊的權重
                    int edgeWeight2 = graph.getEdgeWeight(pp, currentVertex);   //取出邊的權重
                    int edgeWeightPP = graph.getEdgeWeight(pp, neighbor);   //取出邊的權重

                    if(edgeWeightPP < edgeWeight1 + edgeWeight2)
                    {
                        int alt = dist[pp] + edgeWeightPP; //計算新的距離
                        if (alt < dist[neighbor]) { //如果新的距離小於原本的距離
                            dist[neighbor] = alt;   //更新距離
                            pred[neighbor] = pp; //更新前驅
                            open.push({dist[neighbor] + heuristic.get(graph, source, target), neighbor});   //將(距離，鄰居)放入openqueue
                        }
                    }
                    else
                    {
                        int alt = dist[currentVertex] + edgeWeight1; //計算新的距離
                        if (alt < dist[neighbor]) { //如果新的距離小於原本的距離
                            dist[neighbor] = alt;   //更新距離
                            pred[neighbor] = currentVertex; //更新前驅
                            open.push({dist[neighbor] + heuristic.get(graph, source, target), neighbor});   //將(距離，鄰居)放入openqueue
                        }
                    }
                }else
                {
                    int edgeWeight = graph.getEdgeWeight(currentVertex, neighbor);   //取出邊的權重
                    int alt = dist[currentVertex] + edgeWeight; //計算新的距離
                    if (alt < dist[neighbor]) { //如果新的距離小於原本的距離
                        dist[neighbor] = alt;   //更新距離
                        pred[neighbor] = currentVertex; //更新前驅
                        open.push({dist[neighbor] + heuristic.get(graph, source, target), neighbor});   //將(距離，鄰居)放入openqueue
                    }
                }
                if (closedSet.find(neighbor) != closedSet.end()) continue;  //如果鄰居在closedset中，則跳過

            }
        }

        return std::stack<Vertex>();    //回傳空的stack
    }
};

int main() {
    // Create graph
    Graph<Block, int> graph(5);
    vector<Obstacle> obstacles = {Obstacle(2, 2, 0, 4, 4, 2)};

    // Create vertices
    Vertex v1(0), v2(1), v3(2), v4(3), v5(4);
    graph.setVertexProperty(v1, Block(0, 0, 0));
    graph.setVertexProperty(v2, Block(5, 5, 1));
    graph.setVertexProperty(v3, Block(3, 3, 2.5));
    graph.setVertexProperty(v4, Block(3, 3, 5));
    graph.setVertexProperty(v5, Block(4, 4, 3));
    // Add undirected edges
    graph.addUndirectedEdge(v1, v3, 4.9244);
    graph.addUndirectedEdge(v3, v4, 2.5);
    graph.addUndirectedEdge(v4, v5, 2.4494);
    graph.addUndirectedEdge(v5, v2, 3.316);

    // Create heuristic function
    HeuristicA heuristic;

    // Create source and target vertices
    Vertex source = v1;
    Vertex target = v2;

    // Run Theta* algorithm
    std::stack<Vertex> path = ThetaStar::run(source, target, graph, heuristic, obstacles);

    // Print path
    while (!path.empty()) {
        std::cout << path.top().getId() << "->";
        path.pop();
    }
    std::cout << std::endl;

    return 0;
}

