package sa;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

import pathfinding.PathFindingAPI;
import pathfinding.Point;
import pathfinding.Vec3d;


public class SimulatedAnnealingPath {

    private static final double INITIAL_TEMPERATURE = 10000;
    private static final double FINAL_TEMPERATURE = 1e-8;
    private static final double COOLING_RATE = 0.99;
    private static final double STEP_SIZE = 0.05; // 点之间的间距为5厘米
    private static final Random random = new Random();

    private static Coordinate startPoint;
    private static Coordinate endPoint;
    private static List<Coordinate> range1;
    private static List<Coordinate> range2;
    private static List<Coordinate> range3;

    public static void setStartPoint(double x, double y, double z) {
        startPoint = new Coordinate(x, y, z);
    }

    public static void setEndPoint(double x, double y, double z) {
        endPoint = new Coordinate(x, y, z);
    }

    public static void setRange1(double minX, double maxX, double minY, double maxY, double minZ, double maxZ) {
        range1 = generateCoordinatesInRange(minX, maxX, minY, maxY, minZ, maxZ);
    }

    public static void setRange2(double minX, double maxX, double minY, double maxY, double minZ, double maxZ) {
        range2 = generateCoordinatesInRange(minX, maxX, minY, maxY, minZ, maxZ);
    }

    public static void setRange3(double minX, double maxX, double minY, double maxY, double minZ, double maxZ) {
        range3 = generateCoordinatesInRange(minX, maxX, minY, maxY, minZ, maxZ);
    }

    private static List<Coordinate> generateCoordinatesInRange(double minX, double maxX, double minY, double maxY, double minZ, double maxZ) {
        List<Coordinate> points = new ArrayList<>();
        for (double x = minX; x <= maxX; x += STEP_SIZE) {
            for (double y = minY; y <= maxY; y += STEP_SIZE) {
                for (double z = minZ; z <= maxZ; z += STEP_SIZE) {
                    points.add(new Coordinate(x, y, z));
                }
            }
        }
        return points;
    }

    public static PairP<Double, List<Coordinate>> objectiveFunction(Coordinate p1, Coordinate p2, Coordinate p3) {
        List<Coordinate> keyPoints = new ArrayList<>();
        keyPoints.add(startPoint);
        keyPoints.add(p1);
        keyPoints.add(p2);
        keyPoints.add(p3);
        keyPoints.add(endPoint);

        double totalDistance = 0;
        List<Coordinate> coordinatesPath = new ArrayList<>();

        for (int i = 0; i < keyPoints.size() - 1; i++) {
            List<Point> pathSegment = PathFindingAPI.findPath(
                new Point(keyPoints.get(i).x, keyPoints.get(i).y, keyPoints.get(i).z),
                new Point(keyPoints.get(i + 1).x, keyPoints.get(i + 1).y, keyPoints.get(i + 1).z),
                0.08
            );

            for (Point point : pathSegment) {
                coordinatesPath.add(new Coordinate(point.getX(), point.getY(), point.getZ()));
            }

            // Remove the last point of each path segment to avoid duplicates
            if (i < keyPoints.size() - 2) {
                coordinatesPath.remove(coordinatesPath.size() - 1);
            }
        }

        for (int i = 0; i < coordinatesPath.size() - 1; i++) {
            totalDistance += coordinatesPath.get(i).distance(coordinatesPath.get(i + 1));
        }

        System.out.println("Total distance: " + totalDistance);
        System.out.println("Coordinates path: ");
        for (Coordinate coordinate : coordinatesPath) {
            System.out.println(coordinate);
        }

        System.out.println("keyPoints: " + keyPoints);

        return new PairP<>(totalDistance, coordinatesPath);
    }

    public static Coordinate[] simulatedAnnealing() {
        Coordinate[] currentSolution = new Coordinate[3];
        List<Coordinate> currentPath = new ArrayList<>();
        List<Coordinate> bestPath = new ArrayList<>();
        currentSolution[0] = range1.get(random.nextInt(range1.size()));
        currentSolution[1] = range2.get(random.nextInt(range2.size()));
        currentSolution[2] = range3.get(random.nextInt(range3.size()));
        PairP<Double, List<Coordinate>> currentResult = objectiveFunction(currentSolution[0], currentSolution[1], currentSolution[2]);
        double currentEnergy = currentResult.getFirst();
        currentPath = currentResult.getSecond();

        double temperature = INITIAL_TEMPERATURE;

        while (temperature > FINAL_TEMPERATURE) {
            Coordinate[] newSolution = neighbor(currentSolution);
            PairP<Double, List<Coordinate>> newResult = objectiveFunction(newSolution[0], newSolution[1], newSolution[2]);
            double newEnergy = newResult.getFirst();
            List<Coordinate> newPath = newResult.getSecond();

            if (acceptanceProbability(currentEnergy, newEnergy, temperature) > random.nextDouble()) {
                currentSolution = newSolution;
                currentEnergy = newEnergy;
                currentPath = newPath;
            }

            if (bestPath.isEmpty() || currentEnergy < currentResult.getFirst()) {
                bestPath = new ArrayList<>(currentPath);
            }

            temperature *= COOLING_RATE;
        }

        System.out.println("Best path found:");
        for (Coordinate coordinate : bestPath) {
            System.out.println(coordinate);
        }

        System.out.println("keyPoints in the best path: ");
        System.out.println(startPoint);
        System.out.println(currentSolution[0]);
        System.out.println(currentSolution[1]);
        System.out.println(currentSolution[2]);
        System.out.println(endPoint);

        return currentSolution;
    }

    public static double acceptanceProbability(double currentEnergy, double newEnergy, double temperature) {
        if (newEnergy < currentEnergy) {
            return 1.0;
        }
        return Math.exp((currentEnergy - newEnergy) / temperature);
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
}





