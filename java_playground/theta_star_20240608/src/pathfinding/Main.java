package pathfinding;

import java.util.List;

import Point.Point;
import pathfinding.PathfindingMain.PathFindingAPI;

public class Main {
    public static void main(String[] args) {
        Point start = new Point(10.9078d, -10.0293d, 5.1124d);
        Point end = new Point(10.8828d, -8.7924d, 4.3904d);

        List<Point> path = PathFindingAPI.findPath(start, end);

        for (Point p : path) {
            System.out.println("x: " + p.getX() + ", y: " + p.getY() + ", z: " + p.getZ());
        }
    }
}
