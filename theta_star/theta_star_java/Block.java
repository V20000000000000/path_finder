public class Block {
    private int id;
    private double x;
    private double y;
    private double z;
    private double weight;

    // Constructors
    public Block(int id) {
        this.id = id;
        this.x = 0;
        this.y = 0;
        this.z = 0;
    }

    public Block() {
        this(-1, 0, 0, 0);
    }

    public Block(double x, double y, double z) {
        this(-1, x, y, z);
    }

    public Block(int id, double x, double y, double z) {
        this.id = id;
        this.x = x;
        this.y = y;
        this.z = z;
    }

    // Getters
    public int getId() {
        return id;
    }

    public double getWeight() {
        return weight;
    }

    public double getX() {
        return x;
    }

    public double getY() {
        return y;
    }

    public double getZ() {
        return z;
    }

    // Setters
    public void setWeight(double weight) {
        this.weight = weight;
    }

    public void setX(double x) {
        this.x = x;
    }

    public void setY(double y) {
        this.y = y;
    }

    public void setZ(double z) {
        this.z = z;
    }

    public static void main(String[] args) {
        // Example usage
        Block block = new Block(1, 10.0, 20.0, 30.0);
        block.setWeight(5.0);
        
        System.out.println("Block ID: " + block.getId());
        System.out.println("Weight: " + block.getWeight());
        System.out.println("Coordinates: (" + block.getX() + ", " + block.getY() + ", " + block.getZ() + ")");
    }
}

