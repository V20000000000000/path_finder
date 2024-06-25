package theta;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.*;
import graph.*;
import block.Block;

public class Main {
    static double totalLength = 0;
    static int turningCount = 0;

    public static void main(String[] args) throws IOException {
        List<Obstacle> obstacles = Arrays.asList(
            new Obstacle(10.87, -9.5, 4.27, 11.6, -9.45, 4.97),
            new Obstacle(10.25, -9.5, 4.97, 10.87, -9.45, 5.62),
            new Obstacle(10.87, -8.5, 4.97, 11.6, -8.45, 5.62),
            new Obstacle(10.25, -8.5, 4.27, 10.7, -8.45, 4.97),
            new Obstacle(10.87, -7.40, 4.27, 11.6, -7.35, 4.97),
            new Obstacle(10.25, -7.40, 4.97, 10.87, -7.35, 5.62)
        );

        double minX1 = 10.3 + 0.2, minY1 = -10.2 + 0.2, minZ1 = 4.32 + 0.2, maxX1 = 11.55 - 0.2, maxY1 = -6.0 - 0.2, maxZ1 = 5.57 - 0.2;

        int num = 0;
        for (double x = minX1; x <= maxX1; x += 0.05) {
            for (double y = minY1; y <= maxY1; y += 0.05) {
                for (double z = minZ1; z <= maxZ1; z += 0.05) {
                    num++;
                }
            }
        }

        System.out.println("vertex number: " + num);

        Graph<Block, Double> graph = new Graph<>(num);
        Vertex[] vertices = new Vertex[num]; // 用來儲存所有頂點的陣列

        int vertexCount = 0;
        for (double z = minZ1; z <= maxZ1; z += 0.05) {
            for (double y = minY1; y <= maxY1; y += 0.05) {
                for (double x = minX1; x <= maxX1; x += 0.05) {
                    Block block = new Block(x, y, z);
                    VertexProperty<Block> vertexProperty = new VertexProperty<>(block);
                    Vertex vertex = new Vertex(vertexCount);
                    graph.setVertexProperty(vertex, vertexProperty);
                    vertices[vertexCount] = vertex; // 將頂點添加到陣列中
                    vertexCount++;
                }
            }
        }

        System.out.println("vertex number: " + vertexCount);

        // 遍歷所有頂點，計算相鄰頂點，並添加邊
        for (int i = 0; i < num; i++) {
            double x = graph.getVertexProperty(i).getValue().getX();
            double y = graph.getVertexProperty(i).getValue().getY();
            double z = graph.getVertexProperty(i).getValue().getZ();
            for (double dx = -0.05; dx <= 0.05; dx += 0.05) {
                for (double dy = -0.05; dy <= 0.05; dy += 0.05) {
                    for (double dz = -0.05; dz <= 0.05; dz += 0.05) {
                        if (dx == 0 && dy == 0 && dz == 0) continue;
                        double nx = x + dx;
                        double ny = y + dy;
                        double nz = z + dz;
                        if (nx >= minX1 && nx <= maxX1 && ny >= minY1 && ny <= maxY1 && nz >= minZ1 && nz <= maxZ1) {
                            int neighborIndex = (int)((nx - minX1) / 0.05) * ((int)((maxY1 - minY1) / 0.05 + 1) * (int)((maxZ1 - minZ1) / 0.05 + 1)) + (int)((ny - minY1) / 0.05) * (int)((maxZ1 - minZ1) / 0.05 + 1) + (int)((nz - minZ1) / 0.05);
                            double distance = Math.sqrt(dx * dx + dy * dy + dz * dz);
                            graph.addDirectedEdge(i, neighborIndex, distance);
                        }
                    }
                }
            }
        }

        // 找到離給定位置最近的頂點作為 source 和 target
        double sourceX = 10.9, sourceY = -10, sourceZ = 5.12;
        double targetX = 10.55, targetY = -6.75, targetZ = 4.97;
        
        //Vertex source = vertices[15512];
        //Vertex target = vertices[12734];
        Vertex source = findNearestVertex(sourceX, sourceY, sourceZ, vertexCount, graph, vertices);
        Vertex target = findNearestVertex(targetX, targetY, targetZ, vertexCount, graph, vertices);

        HeuristicA heuristic = new HeuristicA();
        Stack<Vertex> path = ThetaStar.run(source, target, graph, heuristic, obstacles);

        List<Block> route = new ArrayList<>();
        int passingNum = path.size();
        int[] passingVertices = new int[passingNum];
        
        Vertex current = path.pop();
        route.add(graph.getVertexProperty(current).getValue());
        int currentIndex = 0;
        double currentX = graph.getVertexProperty(current).getValue().getX();
        double currentY = graph.getVertexProperty(current).getValue().getY();
        double currentZ = graph.getVertexProperty(current).getValue().getZ();

        while (!path.isEmpty()) {
            
            current = path.pop();
            route.add(graph.getVertexProperty(current).getValue());
            double x = graph.getVertexProperty(current).getValue().getX();
            double y = graph.getVertexProperty(current).getValue().getY();
            double z = graph.getVertexProperty(current).getValue().getZ();
            if (x != currentX && y != currentY && z != currentZ) {
            	System.out.println("current id:" + current.getId());
                turningCount++;
            }
            currentX = x;
            currentY = y;
            currentZ = z;
            passingVertices[currentIndex] = current.getId();
            currentIndex++;
        }
        
        for(int i = 0; i < passingNum - 1; i++)
        {
        	System.out.println(graph.getVertexProperty(i).getValue().getX() + "," + graph.getVertexProperty(i).getValue().getY() + "," + graph.getVertexProperty(i).getValue().getZ() + "," + graph.getVertexProperty(i + 1).getValue().getX() + "," + graph.getVertexProperty(i + 1).getValue().getY() + "," + graph.getVertexProperty(i + 1).getValue().getZ());
        }

        System.out.println("total長度:" + totalLength);
        System.out.println("轉彎次數: " + turningCount);
        
        FileWriter fileWriter = new FileWriter("path.txt");
        BufferedWriter bufferedWriter = new BufferedWriter(fileWriter);
        for (Block b : route) {
            bufferedWriter.write(String.format("%.2f, %.2f, %.2f%n", b.getX(), b.getY(), b.getZ()));
        }
        bufferedWriter.close();
    }

    // 找到距離指定位置最近的頂點
    private static Vertex findNearestVertex(double x, double y, double z, int count, Graph<Block, Double> graph, Vertex[] vertices) {
        double minDistance = Double.MAX_VALUE; // 使用 Double 的最大值作為初始最小距離
        double distance;
        int id = 0; // 初始化最近頂點的 id
        for (int i = 0; i < count; i++) {
            distance = Math.sqrt((x - graph.getVertexProperty(i).getValue().getX()) * (x - graph.getVertexProperty(i).getValue().getX()) + (y - graph.getVertexProperty(i).getValue().getY()) * (y - graph.getVertexProperty(i).getValue().getY()) + (z - graph.getVertexProperty(i).getValue().getZ()) * (z - graph.getVertexProperty(i).getValue().getZ()));
            if (distance < minDistance) {
                minDistance = distance; // 更新最小距離
                id = i; // 更新最近頂點的 id
            }
        }
        System.out.println("id:" + id);
        return vertices[id]; // 返回最近的頂點
    }
}

