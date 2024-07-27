package sa;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class SimulatedAnnealingPath {

    private static final double INITIAL_TEMPERATURE = 10000;
    private static final double FINAL_TEMPERATURE = 1e-8;
    private static final double COOLING_RATE = 0.99;
    private static final double STEP_SIZE = 0.05; // 點之間的間距為5公分
    private static final Random random = new Random();

    private static Coordinate startPoint;
    private static Coordinate endPoint;
    private static List<Coordinate> range1;
    private static List<Coordinate> range2;
    private static List<Coordinate> range3;

    public static void setStartPoint(double x, double y) {
        startPoint = new Coordinate(x, y);
    }

    public static void setEndPoint(double x, double y) {
        endPoint = new Coordinate(x, y);
    }

    public static void setRange1(double minX, double maxX, double minY, double maxY) {
        range1 = generateCoordinatesInRange(minX, maxX, minY, maxY);
    }

    public static void setRange2(double minX, double maxX, double minY, double maxY) {
        range2 = generateCoordinatesInRange(minX, maxX, minY, maxY);
    }

    public static void setRange3(double minX, double maxX, double minY, double maxY) {
        range3 = generateCoordinatesInRange(minX, maxX, minY, maxY);
    }

    private static List<Coordinate> generateCoordinatesInRange(double minX, double maxX, double minY, double maxY) {
        List<Coordinate> points = new ArrayList<>();
        for (double x = minX; x <= maxX; x += STEP_SIZE) {
            for (double y = minY; y <= maxY; y += STEP_SIZE) {
                points.add(new Coordinate(x, y));
            }
        }
        return points;
    }

    public static double objectiveFunction(Coordinate p1, Coordinate p2, Coordinate p3) {
        return startPoint.distance(p1) + p1.distance(p2) + p2.distance(p3) + p3.distance(endPoint);
    }

    public static Coordinate[] neighbor(Coordinate[] currentSolution) {
        Coordinate[] newSolution = currentSolution.clone();
        int rangeIndex = random.nextInt(3);
        List<Coordinate> range;
        switch (rangeIndex) {
            case 0:
                range = range1;
                break;
            case 1:
                range = range2;
                break;
            case 2:
                range = range3;
                break;
            default:
                throw new IllegalStateException("Unexpected value: " + rangeIndex);
        }
        newSolution[rangeIndex] = range.get(random.nextInt(range.size()));
        return newSolution;
    }

    public static Coordinate[] simulatedAnnealing() {
        Coordinate[] currentSolution = new Coordinate[3];
        currentSolution[0] = range1.get(random.nextInt(range1.size()));
        currentSolution[1] = range2.get(random.nextInt(range2.size()));
        currentSolution[2] = range3.get(random.nextInt(range3.size()));
        double currentEnergy = objectiveFunction(currentSolution[0], currentSolution[1], currentSolution[2]);

        double temperature = INITIAL_TEMPERATURE;

        while (temperature > FINAL_TEMPERATURE) {
            Coordinate[] newSolution = neighbor(currentSolution);
            double newEnergy = objectiveFunction(newSolution[0], newSolution[1], newSolution[2]);

            if (acceptanceProbability(currentEnergy, newEnergy, temperature) > random.nextDouble()) {
                currentSolution = newSolution;
                currentEnergy = newEnergy;
            }

            temperature *= COOLING_RATE;
        }

        return currentSolution;
    }

    public static double acceptanceProbability(double currentEnergy, double newEnergy, double temperature) {
        if (newEnergy < currentEnergy) {
            return 1.0;
        }
        return Math.exp((currentEnergy - newEnergy) / temperature);
    }
}

