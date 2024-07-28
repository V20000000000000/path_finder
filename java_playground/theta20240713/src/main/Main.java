package main;

import java.util.List;

import pathfinding.*;
import sa.*;


public class Main {

    public static void main(String[] args) {
        SimulatedAnnealingPath.setStartPoint(9.815, -9.806, 4.293);
        SimulatedAnnealingPath.setEndPoint(11.143, -6.607, 4.9654);

        SimulatedAnnealingPath.setRange1(10.42, 10.58, -10.38, 11.48, 4.82, 5.57);
        SimulatedAnnealingPath.setRange2(10.3, 11.55, -9.25, -8.5, 3.76203, 3.96203);
        SimulatedAnnealingPath.setRange3(9.866984, 10.066984, -7.34, -6.365, 4.32, 5.57);

        Coordinate[] bestSolution = SimulatedAnnealingPath.simulatedAnnealing();
        System.out.println("Best solution: ");
        for (Coordinate coordinate : bestSolution) {
            System.out.println(coordinate);
        }
    }
}

