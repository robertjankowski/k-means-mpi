#ifndef __BENCHMARK_OPENMP__
#define __BENCHMARK_OPENMP__

#include "benchmark.h"
#include "kmeans_openmp.h"

void benchmarkOpenMP(MeasureData &&data, int nIteration, const std::string &outputfile)
{
    benchmark(KmeansOpenMP::fit, nIteration, outputfile, data.points, data.k, data.tolerance, data.maxIteration);
}

#endif // __BENCHMARK_OPENMP__