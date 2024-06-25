import java.io.*;
import java.util.*;

class Pair<T, U> {
    T first;
    U second;

    Pair(T first, U second) {
        this.first = first;
        this.second = second;
    }
}

class Obstacle {
    double minX, minY, minZ, maxX, maxY, maxZ;

    Obstacle(double minX, double minY, double minZ, double maxX, double maxY, double maxZ) {
        this.minX = minX;
        this.minY = minY;
        this.minZ = minZ;
        this.maxX = maxX;
        this.maxY = maxY;
        this.maxZ = maxZ;
    }
}

interface HeuristicInterface {
    double get(Graph<Block, Double> graph, Vertex source, Vertex target);
}

class HeuristicA implements HeuristicInterface {
    @Override
    public double get(Graph<Block, Double> graph, Vertex source, Vertex target) {
        double dx = graph.getVertexProperty(target).getX() - graph.getVertexProperty(source).getX();
        double dy = graph.getVertexProperty(target).getY() - graph.getVertexProperty(source).getY();
        double dz = graph.getVertexProperty(target).getZ() - graph.getVertexProperty(source).getZ();
        return Math.sqrt(dx * dx + dy * dy + dz * dz);
    }
}

class Comparator implements Comparator<Pair<Double, Integer>> {
    @Override
    public int compare(Pair<Double, Integer> left, Pair<Double, Integer> right) {
        return Double.compare(left.first, right.first);
    }
}

class ThetaStar {
    static Stack<Vertex> reconstructPath(Vertex source, Vertex target, List<Integer> pred) {
        Stack<Vertex> path = new Stack<>();
        int current = target.getId();
        while (current != -1 && current != source.getId()) {
            path.push(new Vertex(current));
            current = pred.get(current);
        }
        path.push(new Vertex(source.getId()));
        return path;
    }

    static boolean lineOfSight(Vertex source, Vertex target, Graph<Block, Double> graph, List<Obstacle> obstacles) {
        double x0 = graph.getVertexProperty(source).getX();
        double y0 = graph.getVertexProperty(source).getY();
        double z0 = graph.getVertexProperty(source).getZ();
        double x1 = graph.getVertexProperty(target).getX();
        double y1 = graph.getVertexProperty(target).getY();
        double z1 = graph.getVertexProperty(target).getZ();
        double dx = x1 - x0;
        double dy = y1 - y0;
        double dz = z1 - z0;

        for (Obstacle obstacle : obstacles) {
            for (double step = 0; step <= 1; step += 0.0001) {
                double x = x0 + dx * step;
                double y = y0 + dy * step;
                double z = z0 + dz * step;
                if (x > obstacle.minX - 0.2 && x < obstacle.maxX + 0.2 &&
                    y > obstacle.minY - 0.2 && y < obstacle.maxY + 0.2 &&
                    z > obstacle.minZ - 0.2 && z < obstacle.maxZ + 0.2) {
                    return false;
                }
            }
        }
        return true;
    }

    static Stack<Vertex> run(Vertex source, Vertex target, Graph<Block, Double> graph, HeuristicInterface heuristic, List<Obstacle> obstacles) {
        int numVertices = graph.size();
        double[] dist = new double[numVertices];
        Arrays.fill(dist, Double.MAX_VALUE);
        List<Integer> pred = new ArrayList<>(Collections.nCopies(numVertices, -1));
        Set<Integer> closedSet = new HashSet<>();
        dist[source.getId()] = 0;

        PriorityQueue<Pair<Double, Integer>> open = new PriorityQueue<>(new Comparator());

        open.add(new Pair<>(heuristic.get(graph, source, target), source.getId()));

        while (!open.isEmpty()) {
            int currentVertex = open.poll().second;

            if (currentVertex == target.getId()) {
                totalLength += dist[target.getId()];
                return reconstructPath(source, target, pred);
            }

            closedSet.add(currentVertex);

            for (int neighbor : graph.getNeighbors(currentVertex)) {
                if (closedSet.contains(neighbor)) continue;

                int pp = pred.get(currentVertex);

                if (pp != -1 && lineOfSight(new Vertex(pp), new Vertex(neighbor), graph, obstacles)) {
                    double edgeWeight1 = graph.getEdgeWeight(currentVertex, neighbor);
                    double edgeWeight2 = dist[currentVertex] - dist[pp];
                    double dx = graph.getVertexProperty(neighbor).getX() - graph.getVertexProperty(pp).getX();
                    double dy = graph.getVertexProperty(neighbor).getY() - graph.getVertexProperty(pp).getY();
                    double dz = graph.getVertexProperty(neighbor).getZ() - graph.getVertexProperty(pp).getZ();
                    double edgeWeightPP = Math.sqrt(dx * dx + dy * dy + dz * dz);

                    if (edgeWeightPP < edgeWeight1 + edgeWeight2) {
                        double alt = dist[pp] + edgeWeightPP;
                        if (alt < dist[neighbor]) {
                            dist[neighbor] = alt;
                            pred.set(neighbor, pp);
                            open.add(new Pair<>(dist[neighbor] + heuristic.get(graph, new Vertex(neighbor), target), neighbor));
                        }
                    } else {
                        double alt = dist[currentVertex] + edgeWeight1;
                        if (alt < dist[neighbor]) {
                            dist[neighbor] = alt;
                            pred.set(neighbor, currentVertex);
                            open.add(new Pair<>(dist[neighbor] + heuristic.get(graph, new Vertex(neighbor), target), neighbor));
                        }
                    }
                } else {
                    double edgeWeight = graph.getEdgeWeight(currentVertex, neighbor);
                    double alt = dist[currentVertex] + edgeWeight;
                    if (alt < dist[neighbor]) {
                        dist[neighbor] = alt;
                        pred.set(neighbor, currentVertex);
                        open.add(new Pair<>(dist[neighbor] + heuristic.get(graph, new Vertex(neighbor), target), neighbor));
                    }
                }
            }
        }
        return new Stack<>();
    }
}

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
        Map<List<Double>, Integer> vertexLocation = new HashMap<>();

        int vertexCount = 0;
        for (double z = minZ1; z <= maxZ1; z += 0.05) {
            for (double y = minY1; y <= maxY1; y += 0.05) {
                for (double x = minX1; x <= maxX1; x += 0.05) {
                    graph.setVertexProperty(new Vertex(vertexCount), new Block(x, y, z));
                    vertexLocation.put(Arrays.asList(x, y, z), vertexCount);
                    vertexCount++;
                }
            }
        }

        System.out.println("vertex number: " + vertexCount);

        for (int i = 0; i < num; i++) {
            double x = graph.getVertexProperty(i).getX();
            double y = graph.getVertexProperty(i).getY();
            double z = graph.getVertexProperty(i).getZ();
            for (double dx = -0.05; dx <= 0.05; dx += 0.05) {
                for (double dy = -0.05; dy <= 0.05; dy += 0.05) {
                    for (double dz = -0.05; dz <= 0.05; dz += 0.05) {
                        if (dx == 0 && dy == 0 && dz == 0) continue;
                        double nx = x + dx;
                        double ny = y + dy;
                        double nz = z + dz;
                        List<Double> neighbor = Arrays.asList(nx, ny, nz);
                        if (vertexLocation.containsKey(neighbor)) {
                            int neighborIndex = vertexLocation.get(neighbor);
                            double distance = Math.sqrt(dx * dx + dy * dy + dz * dz);
                            graph.addEdge(i, neighborIndex, distance);
                        }
                    }
                }
            }
        }

        Vertex source = new Vertex(vertexLocation.get(Arrays.asList(10.3, -10.2, 4.32)));
        Vertex target = new Vertex(vertexLocation.get(Arrays.asList(11.55, -6.0, 5.57)));

        HeuristicA heuristic = new HeuristicA();
        Stack<Vertex> path = ThetaStar.run(source, target, graph, heuristic, obstacles);

        List<Block> route = new ArrayList<>();
        Vertex current = path.pop();
        route.add(graph.getVertexProperty(current));
        int currentIndex = 0;
        double currentX = graph.getVertexProperty(current).getX();
        double currentY = graph.getVertexProperty(current).getY();
        double currentZ = graph.getVertexProperty(current).getZ();

        while (!path.isEmpty()) {
            currentIndex++;
            current = path.pop();
            route.add(graph.getVertexProperty(current));
            double x = graph.getVertexProperty(current).getX();
            double y = graph.getVertexProperty(current).getY();
            double z = graph.getVertexProperty(current).getZ();
            if (x != currentX && y != currentY && z != currentZ) {
                turningCount++;
            }
            currentX = x;
            currentY = y;
            currentZ = z;
        }

        System.out.println("total length: " + totalLength);
        System.out.println("turning count: " + turningCount);

        FileWriter fileWriter = new FileWriter("path.txt");
        BufferedWriter bufferedWriter = new BufferedWriter(fileWriter);
        for (Block b : route) {
            bufferedWriter.write(String.format("%.2f, %.2f, %.2f%n", b.getX(), b.getY(), b.getZ()));
        }
        bufferedWriter.close();
    }
}

