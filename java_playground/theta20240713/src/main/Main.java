package main;

import java.util.List;

import pathfinding.*;
import pathfinding.PathfindingMain.PathFindingAPI;
import sa.*;
import sa.SimulatedAnnealingPath;


public class Main {

	public static void main(String[] args) {
		List<Point> path = PathFindingAPI.findPath(new Point(10.9078d, -10.0293d, 5.1124d),
				new Point(11.1852d, -6.7607d, 4.8828d), 0.2);

		Log.close();

		System.out.println("Path: ");
		for (Point point : path) {
			System.out.print(point + " ");
		}

		SimulatedAnnealingPath.setStartPoint(0, 0);
		SimulatedAnnealingPath.setEndPoint(10, 10);

		SimulatedAnnealingPath.setRange1(1, 3, 1, 3);
		SimulatedAnnealingPath.setRange2(4, 6, 4, 6);
		SimulatedAnnealingPath.setRange3(7, 9, 7, 9);

		Coordinate[] bestSolution = SimulatedAnnealingPath.simulatedAnnealing();
		System.out.println("最優解: (" + bestSolution[0].x + ", " + bestSolution[0].y + ") -> (" +
							bestSolution[1].x + ", " + bestSolution[1].y + ") -> (" +
							bestSolution[2].x + ", " + bestSolution[2].y + ")");
		System.out.println("目標函數值: " + SimulatedAnnealingPath.objectiveFunction(bestSolution[0], bestSolution[1], bestSolution[2]));
	}
} 
