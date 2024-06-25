import java.util.*;

class VertexProperty<T> {
    T value;

    VertexProperty(T value) {
        this.value = value;
    }

    VertexProperty() {
    }
}

class EdgeProperty<U> {
    U value;

    EdgeProperty(U value) {
        this.value = value;
    }

    EdgeProperty() {
    }

    boolean notEquals(EdgeProperty<U> other) {
        return !Objects.equals(value, other.value);
    }
}

class Vertex {
    private int id;

    Vertex(int id) {
        this.id = id;
    }

    int getId() {
        return id;
    }
}

class Graph<T, U> {
    private List<Map<Integer, Double>> adjacencyList;
    private List<Map<Integer, EdgeProperty<U>>> edgePropertiesMap;
    private EdgeProperty<U> emptyEdgeProperty = new EdgeProperty<>();
    private VertexProperty<T> emptyVertexProperty = new VertexProperty<>();
    private List<VertexProperty<T>> vertexPropertiesMap;

    Graph(int numNodes) {
        adjacencyList = new ArrayList<>(numNodes);
        edgePropertiesMap = new ArrayList<>(numNodes);
        vertexPropertiesMap = new ArrayList<>(numNodes);
        for (int i = 0; i < numNodes; i++) {
            adjacencyList.add(new HashMap<>());
            edgePropertiesMap.add(new HashMap<>());
            vertexPropertiesMap.add(emptyVertexProperty);
        }
    }

    void addBidirectedEdge(int source, int target, double weight1, double weight2) {
        addDirectedEdge(source, target, weight1);
        addDirectedEdge(target, source, weight2);
    }

    void addBidirectedEdge(Vertex source, Vertex target, double weight1, double weight2) {
        addBidirectedEdge(source.getId(), target.getId(), weight1, weight2);
    }

    void addUndirectedEdge(int source, int target, double weight) {
        addBidirectedEdge(source, target, weight, weight);
    }

    void addUndirectedEdge(Vertex source, Vertex target, double weight) {
        addUndirectedEdge(source.getId(), target.getId(), weight);
    }

    void addDirectedEdge(int source, int target, double weight) {
        adjacencyList.get(source).put(target, weight);
    }

    void addDirectedEdge(Vertex source, Vertex target, double weight) {
        addDirectedEdge(source.getId(), target.getId(), weight);
    }

    EdgeProperty<U> getEdgeProperty(int source, int target) {
        return edgePropertiesMap.get(source).getOrDefault(target, emptyEdgeProperty);
    }

    EdgeProperty<U> getEdgeProperty(Vertex source, Vertex target) {
        return getEdgeProperty(source.getId(), target.getId());
    }

    double getEdgeWeight(int source, int target) {
        return adjacencyList.get(source).getOrDefault(target, -1.0);
    }

    double getEdgeWeight(Vertex source, Vertex target) {
        return getEdgeWeight(source.getId(), target.getId());
    }

    List<Integer> getNeighbors(int vertex) {
        return new ArrayList<>(adjacencyList.get(vertex).keySet());
    }

    List<Integer> getNeighbors(Vertex vertex) {
        return getNeighbors(vertex.getId());
    }

    VertexProperty<T> getVertexProperty(int vertex) {
        return vertexPropertiesMap.get(vertex);
    }

    VertexProperty<T> getVertexProperty(Vertex vertex) {
        return getVertexProperty(vertex.getId());
    }

    void setEdgeProperty(int source, int target, EdgeProperty<U> property) {
        if (property.notEquals(emptyEdgeProperty)) {
            edgePropertiesMap.get(source).put(target, property);
        }
    }

    void setEdgeProperty(Vertex source, Vertex target, EdgeProperty<U> property) {
        setEdgeProperty(source.getId(), target.getId(), property);
    }

    void setEdgeWeight(int source, int target, double weight) {
        adjacencyList.get(source).put(target, weight);
    }

    void setEdgeWeight(Vertex source, Vertex target, double weight) {
        setEdgeWeight(source.getId(), target.getId(), weight);
    }

    void setVertexProperty(int vertex, VertexProperty<T> property) {
        vertexPropertiesMap.set(vertex, property);
    }

    void setVertexProperty(Vertex vertex, VertexProperty<T> property) {
        setVertexProperty(vertex.getId(), property);
    }

    int size() {
        return adjacencyList.size();
    }
}

public class Main {
    public static void main(String[] args) {
        Graph<String, Integer> graph = new Graph<>(5);
        graph.setVertexProperty(0, new VertexProperty<>("A"));
        graph.setVertexProperty(1, new VertexProperty<>("B"));
        graph.setVertexProperty(2, new VertexProperty<>("C"));
        graph.setVertexProperty(3, new VertexProperty<>("D"));
        graph.setVertexProperty(4, new VertexProperty<>("E"));

        graph.addDirectedEdge(0, 1, 1.0);
        graph.addDirectedEdge(1, 2, 2.0);
        graph.addDirectedEdge(2, 3, 3.0);
        graph.addDirectedEdge(3, 4, 4.0);
        graph.addDirectedEdge(4, 0, 5.0);

        System.out.println("Graph created with " + graph.size() + " vertices.");
    }
}

