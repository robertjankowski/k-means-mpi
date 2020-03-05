#include <iostream>
#include "measure_time.h"
#include "kmeans.h"
#include "benchmark.h"

int main()
{
    auto points = Observation::getData("../data/n=100000_c=4.csv");

    constexpr int k = 10;
    constexpr double tolerance = 0.001;
    constexpr int maxIteration = 1000;

    // const auto centroids = Kmeans::fit(points, k, tolerance, maxIteration);
    // for (auto &c : centroids)
    //     std::cout << c.getX() << " " << c.getY() << '\n';

    auto elapsed_time = measure<milliseconds>::measure_time(Kmeans::fit, points, k, tolerance, maxIteration).count();
    std::cout << "Elapsed: " << elapsed_time << " ms" << std::endl;

    auto measureData = MeasureData{points, k, tolerance, maxIteration};
    // benchmark(Kmeans::fit, std::move(measureData), 10, "../benchmarks/n=1000_c=2.dat");
}