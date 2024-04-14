package graph;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class Graph<T, U> {
	private List<Map<Integer, Float>> adjacencyList;
	private List<Map<Integer, EdgeProperty<U>>> edgePropertiesMap;
	private EdgeProperty<U> emptyEdgeProperty;
	private VertexProperty<T> emptyVertexProperty;
	private List<VertexProperty<T>> vertexPropertiesMap;

	public Graph(int numNodes) {
		adjacencyList = new ArrayList<>();
		vertexPropertiesMap = new ArrayList<>();
		edgePropertiesMap = new ArrayList<>();
		emptyVertexProperty = new VertexProperty<>(null);
		emptyEdgeProperty = new EdgeProperty<>(null);
		// Initialize adjacency list, vertex properties map, and edge properties map
		for (int i = 0; i < numNodes; i++) {
			adjacencyList.add(new HashMap<>());
			vertexPropertiesMap.add(emptyVertexProperty);
			edgePropertiesMap.add(new HashMap<>());
		}
	}

	public void addBidirectedEdge(int source, int target, float weight1, float weight2) {
		addDirectedEdge(source, target, weight1);
		addDirectedEdge(target, source, weight2);
	}

	public void addBidirectedEdge(Vertex source, Vertex target, float weight1, float weight2) {
		addBidirectedEdge(source.getId(), target.getId(), weight1, weight2);
	}

	public void addDirectedEdge(int source, int target, float weight) {
		adjacencyList.get(source).put(target, weight);
	}

	public void addDirectedEdge(Vertex source, Vertex target, float weight) {
		addDirectedEdge(source.getId(), target.getId(), weight);
	}

	public EdgeProperty<U> getEdgeProperty(int source, int target) {
		return edgePropertiesMap.get(source).get(target);
	}

	public float getEdgeWeight(int source, int target) {
		return adjacencyList.get(source).get(target);
	}

	public float getEdgeWeight(Vertex source, Vertex target) {
		return getEdgeWeight(source.getId(), target.getId());
	}

	public List<Integer> getNeighbors(int vertex) {
		return new ArrayList<>(adjacencyList.get(vertex).keySet());
	}

	public List<Integer> getNeighbors(Vertex vertex) {
		return getNeighbors(vertex.getId());
	}

	public VertexProperty<T> getVertexProperty(int vertex) {
		return vertexPropertiesMap.get(vertex);
	}

	public VertexProperty<T> getVertexProperty(Vertex vertex) {
		return getVertexProperty(vertex.getId());
	}

	public void setEdgeProperty(int source, int target, EdgeProperty<U> property) {
		if (property.notEquals(emptyEdgeProperty)) {
			edgePropertiesMap.get(source).put(target, property);
		}
	}

	public void setVertexProperty(int vertex, VertexProperty<T> property) {
		vertexPropertiesMap.set(vertex, property);
	}

	public void setVertexProperty(Vertex vertex, VertexProperty<T> property) {
		setVertexProperty(vertex.getId(), property);
	}

	public int size() {
		return adjacencyList.size();
	}
}