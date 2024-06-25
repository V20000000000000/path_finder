package graph;

public class VertexProperty<T> {
    T value;

    public VertexProperty(T value) {
        this.value = value;
    }

    public VertexProperty() {
    }

    public T getValue() {
        return value;
    }

    public void setValue(T value) {
        this.value = value;
    }
}
