#ifndef __BENCHMARK__
#define __BENCHMARK__

#include <vector>
#include <iostream>
#include <fstream>
#include <iterator>
#include <utility>
#include "measure_time.h"
#include "kmeans.h"
#include "kmeans_openmp.h"

struct MeasureData
{
    std::vector<Observation> points;
    int k;
    double tolerance;
    int maxIteration;
};

template <typename F, class... Args>
void benchmark(F &&f, int nIteration, const std::string &outputFile, Args &&... args)
{
    std::ofstream file(outputFile);
    if (file.bad())
    {
        std::cerr << "Unable to open file" << std::endl;
        exit(1);
    }

    std::vector<double> elapsedTimes;
    elapsedTimes.reserve(nIteration);
    for (int i = 0; i < nIteration; ++i)
    {
        const auto elapsedTimeWithIterations = measure<milliseconds>::measure_time(f, args...);
        const auto elapsedTime = std::get<0>(elapsedTimeWithIterations).count();
        const auto iterations = std::get<1>(elapsedTimeWithIterations);
        elapsedTimes.push_back(static_cast<double>(elapsedTime) / iterations);
    }

    std::ostream_iterator<double> outputIterator(file, "\n");
    std::copy(elapsedTimes.begin(), elapsedTimes.end(), outputIterator);
}

#endif // __BENCHMARK__