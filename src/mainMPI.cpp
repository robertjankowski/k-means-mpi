#include <iostream>
#include <mpi.h>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <data_filename>\n";
        exit(1);
    }
    MPI_Init(NULL, NULL);
    int worldRank, worldSize;
    MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

    // TODO: implement Kmeans with mpi!
    std::cout << "Rank: " << worldRank << " size: " << worldSize << '\n';

    MPI_Finalize();
}