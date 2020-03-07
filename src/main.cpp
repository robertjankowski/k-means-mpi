#include <iostream>
#include "measure_time.h"
#include "kmeans.h"
#include "benchmark.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <data_filename>\n";
        exit(1);
    }
    auto points = Observation::getData(argv[1]);

    constexpr int k = 10;
    constexpr double tolerance = 0.001;
    constexpr int maxIteration = 1000;

    // const auto centroids = Kmeans::fit(points, k, tolerance, maxIteration);
    // for (auto &c : centroids)
    //     std::cout << c.getX() << " " << c.getY() << '\n';

    auto elapsed_time = measure<milliseconds>::measure_time(Kmeans::fit, points, k, tolerance, maxIteration).count();
    std::cout << "Elapsed: " << elapsed_time << " ms" << std::endl;

    auto measureData = MeasureData{points, k, tolerance, maxIteration};
    // benchmarkSingle(std::move(measureData), 10, "../benchmarks/n=1000_c=2.dat");
}