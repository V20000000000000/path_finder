package algorithm;

import graph.*;

public interface HeuristicInterface {
    double get(Graph<Block, Double> graph, Vertex source, Vertex target);
}
