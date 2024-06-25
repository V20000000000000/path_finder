package theta;

import graph.Vertex;
import block.Block;
import graph.Graph;

public interface HeuristicInterface {
    double get(Graph<Block, Double> graph, Vertex source, Vertex target);
}

