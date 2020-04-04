#!/bin/bash
if [[ $# -ne 4 ]]; then
    echo "Usage: ./run_benchmark [input_data_folder] [clusters] [n_iteration] [output_benchmark_filename]"
    exit 1
fi

data_folder=$1
clusters=$2
n_iteration=$3
benchmark_filename=$4
output_filename=$benchmark_filename"_c="$clusters

make -C src clean
make -C src

echo "Running single thread..."
single_thread_benchmark_filename=$output_filename"_single.csv"
for i in `seq 1 $n_iteration`
do
    ./src/main $data_folder $clusters >> $single_thread_benchmark_filename
done

echo "Running OpenMP version..."
openmp_benchmark_filename=$output_filename"_openmp.csv"
for i in `seq 1 $n_iteration`
do
    ./src/mainOpenMP $data_folder $clusters >> $openmp_benchmark_filename
done

echo "Running MPI version..."
mpi_benchmark_filename=$output_filename"_mpi.csv"
for i in `seq 1 $n_iteration`
do
    mpirun -n 4 ./src/mainMPI $data_folder $clusters >> $mpi_benchmark_filename
done
