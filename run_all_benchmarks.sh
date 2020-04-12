#!/bin/bash
if [[ $# -ne 1 ]]; then
    echo "Usage: ./run_all_benchmarks [n_iteration]"
    exit 1
fi

n_iteration=$1

run_for_cluster() {
    data=$1
    cluster=$2
    for file in $data
    do
        ./run_benchmark.sh $file $cluster $n_iteration
    done
}

echo "Benchmarks for cluster size = 3..."
run_for_cluster "data/*c=3*.csv" 3

echo "Benchmarks for cluster size = 5..."
run_for_cluster "data/*c=5*.csv" 5

echo "Benchmarks for cluster size = 10..."
run_for_cluster "data/*c=10*.csv" 10
