#ifndef __BENCHMARK__
#define __BENCHMARK__

#include <vector>
#include <iostream>
#include <fstream>
#include <iterator>
#include <utility>
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
        const auto res = measure<milliseconds>::measure_time(f, args...);
        const auto elapsedTime = std::get<0>(res).count();
        const auto iteration = std::get<1>(res);
        elapsedTimes.push_back(static_cast<double>(elapsedTime) / iteration);
    }

    std::ostream_iterator<double> outputIterator(file, "\n");
    std::copy(elapsedTimes.begin(), elapsedTimes.end(), outputIterator);
}

void benchmarkSingle(MeasureData &&data, int nIteration, const std::string &outputfile) 
{
    benchmark(Kmeans::fit, nIteration, outputfile, data.points, data.k, data.tolerance, data.maxIteration);
}

void benchmarkOpenMP(MeasureData &&data, int nIteration, const std::string &outputfile)
{
    benchmark(KmeansOpenMP::fit, nIteration, outputfile, data.points, data.k, data.tolerance, data.maxIteration);
}

#endif // __BENCHMARK__