package theta;

class Comparator implements java.util.Comparator<Pair<Double, Integer>> {
    public int compare(Pair<Double, Integer> left, Pair<Double, Integer> right) {
        return left.first.compareTo(right.first);
    }
}