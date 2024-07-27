#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

int main()
{
    // check if gnuplot is installed
    if (std::system("gnuplot --version") == 0)
    {
        std::cout << "\rgnuplot is installed\n";
    }
    else
    {
        throw std::runtime_error("gnuplot is not installed");
    }

    // Generate gnuplot commands
    std::ofstream gnuplot("gnuplot_commands.txt");
    gnuplot << "set terminal png\n";
    gnuplot << "set output 'plot.png'\n";
    gnuplot << "plot '-' with lines\n";
    gnuplot << "1 1\n";
    gnuplot << "2 4\n";
    gnuplot << "3 9\n";
    gnuplot << "4 16\n";
    gnuplot << "e\n";
    gnuplot.close();

    std::system("gnuplot gnuplot_commands.txt");

    std::cout << "\rPlot generated\n";

    return 0;
}