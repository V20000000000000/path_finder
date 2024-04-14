package graph;

public class EdgeProperty<T> {
    T value;

    public boolean notEquals(EdgeProperty<T> other) {
        return !value.equals(other.value);
    }
}
