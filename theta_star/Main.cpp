#include <iostream>
#include <vector>
#include <stack>
#include <unordered_set>
#include <queue>
#include <cmath>
#include <limits>
#include <utility>

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
    virtual double get(const Graph<Block, double>& graph, const Vertex& source, const Vertex& target) const = 0;
};

// HeuristicA is an example heuristic function
class HeuristicA : public HeuristicInterface {
public:
    double get(const Graph<Block, double>& graph, const Vertex& source, const Vertex& target) const override {
        double dx = graph.getVertexProperty(target).value.getX() - graph.getVertexProperty(source).value.getX();
        double dy = graph.getVertexProperty(target).value.getY() - graph.getVertexProperty(source).value.getY();
        double dz = graph.getVertexProperty(target).value.getZ() - graph.getVertexProperty(source).value.getZ();
        return std::sqrt(dx * dx + dy * dy + dz * dz);
    }
};



// 比较函数类
class Comparator {
public:
    // 重载函数调用运算符
    bool operator()(const std::pair<double, int>& left, const std::pair<double, int>& right) const {
        // 按照第一个元素进行比较
        return left.first > right.first;
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

    static bool lineOfSight(const Vertex& source, const Vertex& target, const Graph<Block, double>& graph, const std::vector<Obstacle>& obstacles)
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
        cout << "x0: " << x0 << " y0: " << y0 << " z0: " << z0  << endl << "x1: " << x1 << " y1: " << y1 << " z1: " << z1 << endl;
        cout << "dx: " << dx << " dy: " << dy << " dz: " << dz << endl;
        for (const auto& obstacle : obstacles) {
            for(double step = 0; step < 1; step += 0.001)
            {
                double x = x0 + dx * step;
                double y = y0 + dy * step;
                double z = z0 + dz * step;
                if (x > obstacle.minX && x < obstacle.maxX && y > obstacle.minY && y < obstacle.maxY && z > obstacle.minZ && z < obstacle.maxZ) {
                    cout << "source: v" << source.getId()+1 << " target: v" << target.getId()+1 << " false"<< endl;
                    cout << "x: " << x << " y: " << y << " z: " << z << endl;
                    return false;
                }
            }
        }
        cout << "source: v" << source.getId()+1 << " target: v" << target.getId()+1 << " true"<< endl;
        return true;
    }

    static std::stack<Vertex> run(const Vertex& source, const Vertex& target, const Graph<Block, double>& graph,
                                  const HeuristicInterface& heuristic, const std::vector<Obstacle>& obstacles) {
        std::vector<double> dist(graph.size(), std::numeric_limits<double>::max());
        std::vector<int> pred(graph.size(), -1);
        std::unordered_set<int> closedSet;
        dist[source.getId()] = 0;

        Comparator comparator; // 创建一个 Comparator 类的实例
        std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, Comparator> open(comparator);

        open.push({heuristic.get(graph, source, target), source.getId()});  //將(距離，起點)放入openqueue

        while (!open.empty()) {
            int currentVertex = open.top().second;  //取出openqueue中的最小值
            open.pop(); //將最小值pop出來

            if (currentVertex == target.getId()) {  //如果最小值等於終點，則回傳路徑
                cout << "distance: " << dist[target.getId()] << endl;
                return reconstructPath(source, target, pred);
            }

            closedSet.insert(currentVertex);    //將最小值放入closedset

            for (const auto& neighbor : graph.getNeighbors(currentVertex)) {
                if (closedSet.find(neighbor) != closedSet.end()) continue;  //如果鄰居在closedset中，則跳過
                int pp = pred[currentVertex];  //取出前前驅
                cout << "currentVertex: v" << currentVertex+1 << " neighbor: v" << neighbor+1 << " pp: v" << pp+1 << endl;
                if(pp != -1)
                {
                    if(lineOfSight(pp, neighbor, graph, obstacles))
                    {
                        //cout << "line of sight success" << endl;
                        double edgeWeight1 = graph.getEdgeWeight(currentVertex, neighbor);   //取出邊的權重
                        double edgeWeight2 = dist[currentVertex] - dist[pp];   //取出邊的權重
                        double dx = graph.getVertexProperty(neighbor).value.getX() - graph.getVertexProperty(pp).value.getX();
                        double dy = graph.getVertexProperty(neighbor).value.getY() - graph.getVertexProperty(pp).value.getY();
                        double dz = graph.getVertexProperty(neighbor).value.getZ() - graph.getVertexProperty(pp).value.getZ();
                        double edgeWeightPP = sqrt(dx * dx + dy * dy + dz * dz);   //取出邊的權重

                        cout << "edgeWeight1: " << edgeWeight1 << " edgeWeight2: " << edgeWeight2 << endl;
                        cout << "edgeWeightPP: " << edgeWeightPP << endl;

                        if(edgeWeightPP < edgeWeight1 + edgeWeight2)
                        {
                            double alt = dist[pp] + edgeWeightPP; //計算新的距離
                            if (alt < dist[neighbor]) { //如果新的距離小於原本的距離
                                dist[neighbor] = alt;   //更新距離
                                pred[neighbor] = pp; //更新前驅
                                open.push({dist[neighbor] + heuristic.get(graph, neighbor, target), neighbor});   //將(距離，鄰居)放入openqueue
                            }
                            cout << "PA2 100" << endl;
                        }
                        else
                        {
                            double alt = dist[currentVertex] + edgeWeight1; //計算新的距離
                            if (alt < dist[neighbor]) { //如果新的距離小於原本的距離
                                dist[neighbor] = alt;   //更新距離
                                pred[neighbor] = currentVertex; //更新前驅
                                open.push({dist[neighbor] + heuristic.get(graph, neighbor, target), neighbor});   //將(距離，鄰居)放入openqueue
                            }
                            cout << "PA2 200" << endl;
                        }
                        cout << "distance: " << dist[neighbor] << endl;
                    }else
                    {
                        double edgeWeight = graph.getEdgeWeight(currentVertex, neighbor);   //取出邊的權重
                        double alt = dist[currentVertex] + edgeWeight; //計算新的距離
                        if (alt < dist[neighbor]) { //如果新的距離小於原本的距離
                            dist[neighbor] = alt;   //更新距離
                            pred[neighbor] = currentVertex; //更新前驅
                            open.push({dist[neighbor] + heuristic.get(graph, neighbor, target), neighbor});   //將(距離，鄰居)放入openqueue
                        }
                        cout << "distance: " << dist[neighbor] << endl;
                        cout << "PA2 300" << endl;
                    }
                }else
                {
                    double edgeWeight = graph.getEdgeWeight(currentVertex, neighbor);   //取出邊的權重
                    double alt = dist[currentVertex] + edgeWeight; //計算新的距離
                    if (alt < dist[neighbor]) { //如果新的距離小於原本的距離
                        dist[neighbor] = alt;   //更新距離
                        pred[neighbor] = currentVertex; //更新前驅
                        open.push({dist[neighbor] + heuristic.get(graph, neighbor, target), neighbor});   //將(距離，鄰居)放入openqueue
                    }
                    cout << "distance: " << dist[neighbor] << endl;
                    cout << "PA2 400" << endl;
                }
                cout << endl;
            }
        }
        cout << endl;
        return std::stack<Vertex>();    //回傳空的stack
    }
};

int main() {
    // Create graph
    Graph<Block, double> graph(13);
    vector<Obstacle> obstacles = {Obstacle(1, 2, 0, 2, 3, 1)};

    // Create vertices
    Vertex v1(0), v2(1), v3(2), v4(3), v5(4), v6(5), v7(6), v8(7), v9(8), v10(9), v11(10), v12(11), v13(12);
    
    graph.setVertexProperty(v1, Block(0, 0, 0.5));
    graph.setVertexProperty(v2, Block(1, 1, 0.5));
    graph.setVertexProperty(v3, Block(2, 1, 0.5)); 
    graph.setVertexProperty(v4, Block(3, 1, 0.5));
    graph.setVertexProperty(v5, Block(1, 2, 0.5));
    graph.setVertexProperty(v6, Block(2, 2, 0.5));
    graph.setVertexProperty(v7, Block(3, 2, 0.5));
    graph.setVertexProperty(v8, Block(1, 3, 0.5));
    graph.setVertexProperty(v9, Block(2, 3, 0.5));
    graph.setVertexProperty(v10, Block(3, 3, 0.5));
    graph.setVertexProperty(v11, Block(1, 4, 0.5));
    graph.setVertexProperty(v12, Block(2, 4, 0.5));
    graph.setVertexProperty(v13, Block(3, 4, 0.5));
    // Add undirected edges
    graph.addUndirectedEdge(v1, v2, 1.44);
    graph.addUndirectedEdge(v2, v3, 1);
    graph.addUndirectedEdge(v3, v4, 1);
    graph.addUndirectedEdge(v5, v6, 1);
    graph.addUndirectedEdge(v6, v7, 1);
    graph.addUndirectedEdge(v8, v9, 1);
    graph.addUndirectedEdge(v9, v10, 1);
    graph.addUndirectedEdge(v2, v5, 1);
    graph.addUndirectedEdge(v3, v6, 1);
    graph.addUndirectedEdge(v4, v7, 1);
    graph.addUndirectedEdge(v5, v8, 1);
    graph.addUndirectedEdge(v6, v9, 1);
    graph.addUndirectedEdge(v7, v10, 1);
    graph.addUndirectedEdge(v8, v11, 1);
    graph.addUndirectedEdge(v9, v12, 1);
    graph.addUndirectedEdge(v10, v13, 1);
    graph.addUndirectedEdge(v11, v12, 1);
    graph.addUndirectedEdge(v12, v13, 1);

    // Create heuristic function
    HeuristicA heuristic;

    // Create source and target vertices
    Vertex source = v11;
    Vertex target = v2;

    // Run Theta* algorithm
    std::stack<Vertex> path = ThetaStar::run(source, target, graph, heuristic, obstacles);

    cout << "---------------" << endl;
    cout << "source: v" << source.getId()+1 << " target: v" << target.getId()+1 << endl;
    // Print path
    while (!path.empty()) {
        std::cout << " v" << path.top().getId() + 1 << "->";
        path.pop();
    }
    std::cout << std::endl;

    return 0;
}

