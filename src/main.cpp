#include <iostream>
#include "measure_time.h"
#include "kmeans.h"
#include "benchmark_single.h"
#include "utils.h"

int main(int argc, char *argv[])
{
    srand(time(NULL));

    const auto fileWithK = Utils::loadFilenameWithKClusters(argc, argv);
    const std::string inputFile = std::get<0>(fileWithK);
    const int k = std::get<1>(fileWithK);

    auto points = Observation::getData(inputFile);
    constexpr double tolerance = 0.001;
    constexpr int maxIteration = 1000;

#define SHOW_CLUSTERS false
#if SHOW_CLUSTERS
    const auto observationWithIteration = Kmeans::fit(points, k, tolerance, maxIteration);
    const auto centroids = std::get<0>(observationWithIteration);
    for (auto &c : centroids)
        std::cout << c.getX() << " " << c.getY() << '\n';
#endif
    const auto elapsed_time = measure<microseconds>::measure_time(Kmeans::fit, points, k, tolerance, maxIteration)
                                  .first
                                  .count();
    std::cout << static_cast<double>(elapsed_time) / 1000 << std::endl;

    // auto measureData = MeasureData{points, k, tolerance, maxIteration};
    // const auto outputFileName = Utils::split(inputFile, '/').at(2);
    // const auto outFile = "../benchmarks/o3_single_" + outputFileName;
    // benchmarkSingle(std::move(measureData), 10, outFile);
}