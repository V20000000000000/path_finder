package algorithm;

import java.util.Comparator;
import pathfinding.Pair;

public class VertexComparator implements Comparator<Pair<Double, Integer>> {
    @Override
    public int compare(Pair<Double, Integer> left, Pair<Double, Integer> right) {
        return left.getFirst().compareTo(right.getFirst());
    }
}
