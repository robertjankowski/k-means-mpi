#ifndef __BENCHMARK_SINGLE__
#define __BENCHMARK_SINGLE__

#include "benchmark.h"

void benchmarkSingle(MeasureData &&data, int nIteration, const std::string &outputfile)
{
    benchmark(Kmeans::fit, nIteration, outputfile, data.points, data.k, data.tolerance, data.maxIteration);
}

#endif // __BENCHMARK_SINGLE__