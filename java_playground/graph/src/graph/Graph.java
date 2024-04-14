package graph;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class Graph<T, U> {
    private int numNodes;
    private List<Map<Integer, Float>> adjacencyList;
    private List<VertexProperty<T>> vertexPropertiesMap;
    private List<Map<Integer, EdgeProperty<U>>> edgePropertiesMap;
    private VertexProperty<T> emptyVertexProperty;
    private EdgeProperty<U> emptyEdgeProperty;

    public Graph(int numNodes) {
        this.numNodes = numNodes;
        adjacencyList = new ArrayList<>();
        vertexPropertiesMap = new ArrayList<>();
        edgePropertiesMap = new ArrayList<>();
        emptyVertexProperty = new VertexProperty<>();
        emptyEdgeProperty = new EdgeProperty<>();
        for (int i = 0; i < numNodes; i++) {
            adjacencyList.add(new HashMap<>());
            vertexPropertiesMap.add(emptyVertexProperty);
            edgePropertiesMap.add(new HashMap<>());
        }
    }

    public void addVertex(Vertex vertex) {
        adjacencyList.set(vertex.id, new HashMap<>());
        vertexPropertiesMap.set(vertex.id, emptyVertexProperty);
    }

    public void setVertexProperty(int vertex, VertexProperty<T> property) {
        vertexPropertiesMap.set(vertex, property);
    }

    public void setVertexProperty(Vertex vertex, VertexProperty<T> property) {
        setVertexProperty(vertex.id, property);
    }

    public VertexProperty<T> getVertexProperty(int vertex) {
        return vertexPropertiesMap.get(vertex);
    }

    public VertexProperty<T> getVertexProperty(Vertex vertex) {
        return getVertexProperty(vertex.id);
    }

    public void addDirectedEdge(int source, int target, float weight) {
        adjacencyList.get(source).put(target, weight);
    }

    public void addDirectedEdge(Vertex source, Vertex target, float weight) {
        addDirectedEdge(source.id, target.id, weight);
    }

    public void addBidirectedEdge(int source, int target, float weight1, float weight2) {
        addDirectedEdge(source, target, weight1);
        addDirectedEdge(target, source, weight2);
    }

    public void addBidirectedEdge(Vertex source, Vertex target, float weight1, float weight2) {
        addBidirectedEdge(source.id, target.id, weight1, weight2);
    }

    public void setEdgeProperty(int source, int target, EdgeProperty<U> property) {
        if (!property.notEquals(emptyEdgeProperty)) {
            edgePropertiesMap.get(source).put(target, property);
        }
    }

    public EdgeProperty<U> getEdgeProperty(int source, int target) {
        return edgePropertiesMap.get(source).get(target);
    }

    public List<Integer> getNeighbors(int vertex) {
        List<Integer> neighbors = new ArrayList<>();
        for (Integer neighbor : adjacencyList.get(vertex).keySet()) {
            neighbors.add(neighbor);
        }
        return neighbors;
    }

    public List<Integer> getNeighbors(Vertex vertex) {
        return getNeighbors(vertex.id);
    }

    public float getEdgeWeight(int source, int target) {
        if (adjacencyList.get(source).containsKey(target)) {
            return adjacencyList.get(source).get(target);
        } else {
            return -1.0f;
        }
    }

    public float getEdgeWeight(Vertex source, Vertex target) {
        return getEdgeWeight(source.id, target.id);
    }

    public int size() {
        return adjacencyList.size();
    }
}
