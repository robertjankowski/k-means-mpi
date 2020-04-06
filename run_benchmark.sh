#!/bin/bash
if [[ $# -ne 3 ]]; then
    echo "Usage: ./run_benchmark [input_data_folder] [clusters] [n_iteration]"
    exit 1
fi

data_folder=$1
clusters=$2
n_iteration=$3
cluster_name=$(grep -Eo 'c=[0-9]+' <<< $data_folder)
output_filename=$(grep -Eo 'n=[0-9]+' <<< $data_folder)
output_filename=$output_filename"_"$cluster_name".csv"

run_simulation() {
    n_iteration="$1"
    run_script="$2"    
    for i in `seq 1 $n_iteration`
    do
        eval $run_script
    done
}

# make -C src clean
make -C src

echo "Running single thread..."
single_thread_benchmark_filename="benchmarks/single_"$output_filename
run_single="./src/main "$data_folder" "$clusters" >> "$single_thread_benchmark_filename
run_simulation $n_iteration "$run_single"

echo "Running OpenMP version..."
openmp_benchmark_filename="benchmarks/openmp_"$output_filename
run_openmp="./src/mainOpenMP "$data_folder" "$clusters" >> "$openmp_benchmark_filename
run_simulation $n_iteration "$run_openmp"

echo "Running MPI version..."
mpi_benchmark_filename="benchmarks/mpi_"$output_filename
run_mpi="mpirun -n 4 ./src/mainMPI "$data_folder" "$clusters" >> "$mpi_benchmark_filename
run_simulation $n_iteration "$run_mpi"
