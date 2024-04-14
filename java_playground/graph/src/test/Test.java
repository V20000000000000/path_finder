package test;

import graph.*;

public class Test {

	public static void main(String[] args) {
		Graph<Box, Integer> graph = new Graph<>(5);

		Vertex v1 = new Vertex(0);
		Vertex v2 = new Vertex(1);
		Vertex v3 = new Vertex(2);
		Vertex v4 = new Vertex(3);
		Vertex v5 = new Vertex(4);

		graph.addDirectedEdge(v1, v2, 1);
		graph.addDirectedEdge(v1, v3, 2);
		graph.addDirectedEdge(v2, v3, 3);
		graph.addDirectedEdge(v3, v4, 4);
		graph.addDirectedEdge(v4, v5, 5);

		// Set vertex property
		graph.setVertexProperty(0, new VertexProperty<>(new Box(0, 0, 0, 1, 1, 1)));
		graph.setVertexProperty(1, new VertexProperty<>(new Box(1, 1, 1, 2, 2, 2)));
		graph.setVertexProperty(2, new VertexProperty<>(new Box(2, 2, 2, 3, 3, 3)));
		graph.setVertexProperty(3, new VertexProperty<>(new Box(3, 3, 3, 4, 4, 4)));
		graph.setVertexProperty(4, new VertexProperty<>(new Box(4, 4, 4, 5, 5, 5)));

		// Set edge property
		graph.setEdgeProperty(0, 1, new EdgeProperty<>(1));
		graph.setEdgeProperty(0, 2, new EdgeProperty<>(2));
		graph.setEdgeProperty(1, 2, new EdgeProperty<>(3));
		graph.setEdgeProperty(2, 3, new EdgeProperty<>(4));
		graph.setEdgeProperty(3, 4, new EdgeProperty<>(5));

		// Get vertex property
		System.out.println("Width: " + graph.getVertexProperty(0).getValue().getWidth());

		// Get edge property
		System.out.println("Edge weight: " + graph.getEdgeWeight(0, 1));

		// Get neighbors
		System.out.println("Neighbors: " + graph.getNeighbors(0));

		// Set vertex property
		graph.setVertexProperty(v1, new VertexProperty<>(new Box(10, 10, 10, 1, 1, 1)));
		System.out.println("X: " + graph.getVertexProperty(v1).getValue().getX());

		// Set edge property
		graph.setEdgeProperty(v1, v2, new EdgeProperty<>(10));
		System.out.println("Edge weight: " + graph.getEdgeWeight(v1, v2));
	}

}
