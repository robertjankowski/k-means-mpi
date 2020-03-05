#include <iostream>
#include <mpi.h>

int main()
{
    MPI_Init(NULL, NULL);
    int worldRank, worldSize;
    MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

    // TODO: implement Kmeans with mpi!
    std::cout << "Rank: " << worldRank << " size: " << worldSize << '\n';

    MPI_Finalize();
}