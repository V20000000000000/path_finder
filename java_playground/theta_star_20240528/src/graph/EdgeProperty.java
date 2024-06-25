package graph;

import java.util.Objects;

public class EdgeProperty<U> {
    U value;

    public EdgeProperty(U value) {
        this.value = value;
    }

    public EdgeProperty() {
    }

    public boolean notEquals(EdgeProperty<U> other) {
        return !Objects.equals(value, other.value);
    }

    public U getValue() {
        return value;
    }

    public void setValue(U value) {
        this.value = value;
    }
}
