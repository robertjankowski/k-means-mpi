#include <iostream>
#include "measure_time.h"
#include "kmeans_openmp.h"
#include "utils.h"
#include "benchmark.h"

int main(int argc, char *argv[])
{
    srand(time(NULL));
    
    const auto fileWithK = Utils::loadFilenameWithK(argc, argv);
    const std::string inputFile = std::get<0>(fileWithK);
    int k = std::get<1>(fileWithK);
    
    auto points = Observation::getData(inputFile);
    constexpr double tolerance = 0.001;
    constexpr int maxIteration = 1000;

#define SHOW_CLUSTERS FALSE
#if SHOW_CLUSTERS
    const auto centroids = KmeansOpenMP::fit(points, k, tolerance, maxIteration);
    for (auto &c : centroids)
        std::cout << c.getX() << " " << c.getY() << '\n';
#endif
    // auto elapsed_time = measure<milliseconds>::measure_time(Kmeans::fit, points, k, tolerance, maxIteration).count();
    // std::cout << "Elapsed: " << elapsed_time << " ms" << std::endl;

    auto measureData = MeasureData{points, k, tolerance, maxIteration};
    const auto outFile = "../benchmarks/openmp_" + Utils::split(inputFile, '/').at(2); // get name of input file
    benchmarkOpenMP(std::move(measureData), 10, outFile);
}