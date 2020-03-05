#ifndef __BENCHMARK__
#define __BENCHMARK__

#include <vector>
#include <iostream>
#include <fstream>
#include <iterator>
#include "kmeans.h"

struct MeasureData
{
    std::vector<Observation> points;
    int k;
    double tolerance;
    int maxIteration;
};

template <typename F>
void benchmark(F &&f, MeasureData &&data, int nIteration, const std::string &outputFile)
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
        const auto elapsedTime = measure<milliseconds>::measure_time(f, data.points, data.k, data.tolerance, data.maxIteration).count();
        elapsedTimes.push_back(elapsedTime);
    }

    std::ostream_iterator<double> outputIterator(file, "\n");
    std::copy(elapsedTimes.begin(), elapsedTimes.end(), outputIterator);
}

#endif // __BENCHMARK__