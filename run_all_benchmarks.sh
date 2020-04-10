#!/bin/bash
if [[ $# -ne 1 ]]; then
    echo "Usage: ./run_all_benchmarks [n_iteration]"
    exit 1
fi

n_iteration=$1

echo "Benchmarks for cluster size = 3..."
for file in data/*c=3*.csv
do
    ./run_benchmark.sh $file 3 $n_iteration
done

echo "Benchmarks for cluster size = 5..."
for file in data/*c=5*.csv
do
    ./run_benchmark.sh $file 5 $n_iteration
done

echo "Benchmarks for cluster size = 10..."
for file in data/*c=10*.csv
do
    ./run_benchmark.sh $file 10 $n_iteration
done
