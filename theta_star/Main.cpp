#include <iostream>
#include <vector>
#include <stack>
#include <unordered_set>
#include <queue>
#include <cmath>
#include <limits>
#include <utility>
#include <fstream>
#include <map>
#include <sstream>

#include "Graph.hpp"
#include "Block.hpp"

using namespace std;

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

static double totalLength = 0;
static int turningCount = 0;

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
        for (const auto& obstacle : obstacles) {
            for(double step = 0; step <= 1; step += 0.0001)
            {
                double x = x0 + dx * step;
                double y = y0 + dy * step;
                double z = z0 + dz * step;
                if (x > obstacle.minX - 0.2 && x < obstacle.maxX + 0.2 && y > obstacle.minY-0.2 && y < obstacle.maxY + 0.2 && z > obstacle.minZ-0.2 && z < obstacle.maxZ + 0.2) {
                    return false;
                }
            }
        }
        return true;
        //return false;
    }

    static std::stack<Vertex> run(const Vertex& source, const Vertex& target, const Graph<Block, double>& graph,
                                  const HeuristicInterface& heuristic, const std::vector<Obstacle>& obstacles) {
        ofstream outputNeighborFile("neighbor.txt");
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
                //cout << "distance: " << dist[target.getId()] << endl;
                return reconstructPath(source, target, pred);
            }

            closedSet.insert(currentVertex);    //將最小值放入closedset

            for (const auto& neighbor : graph.getNeighbors(currentVertex)) {
                // 輸出鄰居
                for(int i = 0; i < graph.getNeighbors(currentVertex).size(); i++)
                {
                    cout << "v" << graph.getNeighbors(currentVertex)[i]+1 << " ";
                    outputNeighborFile << graph.getNeighbors(currentVertex)[i]+1 << " ";
                }
                cout << endl;
                outputNeighborFile << endl;
                if (closedSet.find(neighbor) != closedSet.end()) continue;  //如果鄰居在closedset中，則跳過
                int pp = pred[currentVertex];  //取出前前驅
                cout << "------------------------------------" << endl;
                cout << "currentVertex: v" << currentVertex+1 << " neighbor: v" << neighbor+1 << " pred[currentVertex]: v" << pp+1 << endl;
                if(pp != -1)
                {
                    if(lineOfSight(pp, neighbor, graph, obstacles))
                    {
                        cout << "line of sight success" << endl;
                        double edgeWeight1 = graph.getEdgeWeight(currentVertex, neighbor);   //取出邊的權重
                        double edgeWeight2 = dist[currentVertex] - dist[pp];   //取出邊的權重
                        double dx = graph.getVertexProperty(neighbor).value.getX() - graph.getVertexProperty(pp).value.getX();
                        double dy = graph.getVertexProperty(neighbor).value.getY() - graph.getVertexProperty(pp).value.getY();
                        double dz = graph.getVertexProperty(neighbor).value.getZ() - graph.getVertexProperty(pp).value.getZ();
                        double edgeWeightPP = sqrt(dx * dx + dy * dy + dz * dz);   //取出邊的權重

                        //cout << "edgeWeightPP " << edgeWeightPP << ", edgeWeight1 + edgeWeight2 " << edgeWeight1 + edgeWeight2 << endl;

                        if(edgeWeightPP < edgeWeight1 + edgeWeight2)    //如果新的邊的權重小於原本的權重
                        {
                            double alt = dist[pp] + edgeWeightPP; //計算新的距離
                            if (alt < dist[neighbor]) { //如果新的距離小於原本的距離
                                dist[neighbor] = alt;   //更新距離
                                pred[neighbor] = pp; //更新前驅
                                open.push({dist[neighbor] + heuristic.get(graph, neighbor, target), neighbor});   //將(距離，鄰居)放入openqueue
                                cout << "actually success" << endl;
                            }
                        }
                        else
                        {
                            double alt = dist[currentVertex] + edgeWeight1; //計算新的距離
                            if (alt < dist[neighbor]) { //如果新的距離小於原本的距離
                                dist[neighbor] = alt;   //更新距離
                                pred[neighbor] = currentVertex; //更新前驅
                                open.push({dist[neighbor] + heuristic.get(graph, neighbor, target), neighbor});   //將(距離，鄰居)放入openqueue
                                cout << "actually fail" << endl;
                            }
                        }
                    }else
                    {
                        cout << "line of sight fail" << endl;
                        double edgeWeight = graph.getEdgeWeight(currentVertex, neighbor);   //取出邊的權重
                        double alt = dist[currentVertex] + edgeWeight; //計算新的距離
                        if (alt < dist[neighbor]) { //如果新的距離小於原本的距離
                            dist[neighbor] = alt;   //更新距離
                            pred[neighbor] = currentVertex; //更新前驅
                            open.push({dist[neighbor] + heuristic.get(graph, neighbor, target), neighbor});   //將(距離，鄰居)放入openqueue
                        }
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
                }
                //cout << endl;
            }
        }

        outputNeighborFile.close();
        
        //cout << endl;
        return std::stack<Vertex>();    //回傳空的stack
    }
};

int main() {
    vector<Obstacle> obstacles = {
    Obstacle(10, 2, 0, 12, 7, 0),};

    //KIZ1: 
    double minX1 = 0, minY1 = 0, minZ1 = 0, maxX1 = 20, maxY1 = 12, maxZ1 = 0;

    int num = 0;

    for(double x = minX1; x <= maxX1; x += 0.05)
    {
        for(double y = minY1; y <= maxY1; y += 0.05)
        {
            for(double z = minZ1; z <= maxZ1; z += 0.05)
            {
                num++;
            }
        }
    }

    cout << "vertex number: " << num << endl;

    // Create graph
    Graph<Block, double> graph(num);
    map<vector<double>, int> vertexLocation;

    int vertexCount = 0;
    // set vertices property
    for(double z = minZ1; z <= maxZ1; z += 0.05)
    {
        for(double y = minY1; y <= maxY1; y += 0.05)
        {
            for(double x = minX1; x <= maxX1; x += 0.05)
            {
                graph.setVertexProperty(Vertex(vertexCount), Block(x, y, z));
                vertexLocation[{x, y, z}] = vertexCount;
                vertexCount++;
            }
        }
    }

    cout << "vertex number: " << vertexCount << endl;

    //Add undirected edges
    for(int i = 0; i < num; i++)
    {
        double x = graph.getVertexProperty(i).value.getX();
        double y = graph.getVertexProperty(i).value.getY();
        double z = graph.getVertexProperty(i).value.getZ();

        bool inObstacleFlag = false;

        for (const auto& obstacle : obstacles)
        {
            if (x > obstacle.minX && x < obstacle.maxX && y > obstacle.minY && y < obstacle.maxY && z > obstacle.minZ && z < obstacle.maxZ) {
                inObstacleFlag = true;
                break;
            }
        }

        if(inObstacleFlag == true)
        {
            continue;
        }
        else
        {
            if (vertexLocation.find({x, y, z}) == vertexLocation.end()) 
            {
                continue;
            }
            else
            {
                if(vertexLocation.find({x+0.05, y, z}) != vertexLocation.end())
                {
                    graph.addDirectedEdge(i, vertexLocation[{x+0.05, y, z}], 0.05);
                }
                if(vertexLocation.find({x, y+0.05, z}) != vertexLocation.end())
                {
                    graph.addDirectedEdge(i, vertexLocation[{x, y+0.05, z}], 0.05);
                }
                if(vertexLocation.find({x, y, z+0.05}) != vertexLocation.end())
                {
                    graph.addDirectedEdge(i, vertexLocation[{x, y, z+0.05}], 0.05);
                }
                if(vertexLocation.find({x-0.05, y, z}) != vertexLocation.end())
                {
                    graph.addDirectedEdge(i, vertexLocation[{x-0.05, y, z}], 0.05);
                }
                if(vertexLocation.find({x, y-0.05, z}) != vertexLocation.end())
                {
                    graph.addDirectedEdge(i, vertexLocation[{x, y-0.05, z}], 0.05);
                }
                if(vertexLocation.find({x, y, z-0.05}) != vertexLocation.end())
                {
                    graph.addDirectedEdge(i, vertexLocation[{x, y, z-0.05}], 0.05);
                }
            }
        }
    }

    // Create heuristic function
    HeuristicA heuristic;

    // Create source and target vertices
    ifstream inputfile("input.txt");

    if(!inputfile)
    {
        cout << "File not found" << endl;
        return 0;
    }

    ofstream outputvertexLocFile("vertexLoc.txt");

    for (int i = 0; i < num; i++)
    {
        outputvertexLocFile << i << " " << graph.getVertexProperty(i).value.getX() - 0.025 << " " << graph.getVertexProperty(i).value.getY() - 0.025 << " " << graph.getVertexProperty(i).value.getZ() - 0.025 << " " << graph.getVertexProperty(i).value.getX() + 0.025 << " " << graph.getVertexProperty(i).value.getY() + 0.025 << " " << graph.getVertexProperty(i).value.getZ() + 0.025 << endl;
    }

    outputvertexLocFile.close();

    

    ofstream outputfile("Paths.csv");

    outputfile << "xmin" << "," << "ymin" << "," << "zmin" << "," << "xmax" << "," << "ymax" << "," << "zmax" << endl;

    double sx, sy, sz, tx, ty, tz;
    //input dataParse
    vector<vector<double>> locationData;

    std::string line;
    while (std::getline(inputfile, line))
    {
        std::istringstream iss(line);
        double x, y, z;
        if (iss >> x >> y >> z)
        {
            locationData.push_back({x, y, z});
        }
        else
        {
            cout << "input format error" << endl;
        }
    }
    
    for(unsigned int k = 0; k < locationData.size() - 1; k++)
    {
        
        sx = locationData[k][0];
        sy = locationData[k][1];
        sz = locationData[k][2];
    
        tx = locationData[k+1][0];
        ty = locationData[k+1][1];
        tz = locationData[k+1][2];

        //find nearest source and nearest target vertex
        double minDistance1 = 1000000;
        double minDistance2 = 1000000;
        int s = 0, t = 0;

        for(int i = 0; i < num; i++)
        {
            double x = graph.getVertexProperty(i).value.getX();
            double y = graph.getVertexProperty(i).value.getY();
            double z = graph.getVertexProperty(i).value.getZ();

            double distance = sqrt((sx - x) * (sx - x) + (sy - y) * (sy - y) + (sz - z) * (sz - z));
            if(distance < minDistance1)
            {
                minDistance1 = distance;
                s = i;
            }

            distance = sqrt((tx - x) * (tx - x) + (ty - y) * (ty - y) + (tz - z) * (tz - z));
            if(distance < minDistance2)
            {
                minDistance2 = distance;
                t = i;
            }
        }

        Vertex source = s;
        Vertex target = t;

        //is source and target in obstacle
        for (const auto& obstacle : obstacles) {
            if (graph.getVertexProperty(source).value.getX() > obstacle.minX && graph.getVertexProperty(source).value.getX() < obstacle.maxX && graph.getVertexProperty(source).value.getY() > obstacle.minY && graph.getVertexProperty(source).value.getY() < obstacle.maxY && graph.getVertexProperty(source).value.getZ() > obstacle.minZ && graph.getVertexProperty(source).value.getZ() < obstacle.maxZ) {
                cout << "source in obstacle" << endl;
                return 0;
            }
            if (graph.getVertexProperty(target).value.getX() > obstacle.minX && graph.getVertexProperty(target).value.getX() < obstacle.maxX && graph.getVertexProperty(target).value.getY() > obstacle.minY && graph.getVertexProperty(target).value.getY() < obstacle.maxY && graph.getVertexProperty(target).value.getZ() > obstacle.minZ && graph.getVertexProperty(target).value.getZ() < obstacle.maxZ) {
                cout << "target in obstacle" << endl;
                return 0;
            }
        }

        // Run Theta* algorithm
        std::stack<Vertex> path = ThetaStar::run(source, target, graph, heuristic, obstacles);

        vector<vector<double>> passingVertex = {};

        cout << "---------------" << endl;
        cout << "source: v" << source.getId() << " target: v" << target.getId() << endl;
        // Print path
        while (!path.empty()) {
            std::cout << " v" << path.top().getId()
            << "(x = " << graph.getVertexProperty(path.top().getId()).value.getX()
            << ", y = " << graph.getVertexProperty(path.top().getId()).value.getY()
            << ", z = " << graph.getVertexProperty(path.top().getId()).value.getZ() << ")"
            << "->" << endl;
            passingVertex.push_back({graph.getVertexProperty(path.top().getId()).value.getX(), graph.getVertexProperty(path.top().getId()).value.getY(), graph.getVertexProperty(path.top().getId()).value.getZ()});
            
            path.pop();
        }

        for(unsigned int i = 0; i < passingVertex.size() - 1; i++)
        {
            outputfile << passingVertex[i][0] << "," << passingVertex[i][1] << "," << passingVertex[i][2] << "," << passingVertex[i+1][0] << "," << passingVertex[i+1][1] << "," << passingVertex[i+1][2] << endl;
        }
        
        for(unsigned int i = 0; i < passingVertex.size() - 1; i++)
        {
            double dx = passingVertex[i+1][0] - passingVertex[i][0];
            double dy = passingVertex[i+1][1] - passingVertex[i][1];
            double dz = passingVertex[i+1][2] - passingVertex[i][2];
            totalLength += sqrt(dx * dx + dy * dy + dz * dz);
            turningCount++;
        }
    }

    
    cout << "Distance: " << totalLength << endl;
    cout << "turningCount: " << turningCount - 1 << endl;
    outputfile.close();
    inputfile.close();
    std::cout << std::endl;

    return 0;
}

