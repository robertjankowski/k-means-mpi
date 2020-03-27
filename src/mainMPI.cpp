#include <iostream>
#include <mpi.h>
#include "utils.h"
#include "observation.h"
#include "kmeans_mpi.h"
#include "kmeans.h"

int main(int argc, char *argv[])
{
    const auto fileWithK = Utils::loadFilenameWithKClusters(argc, argv);
    const std::string inputFile = std::get<0>(fileWithK);
    const int k = std::get<1>(fileWithK);
    
    constexpr double tolerance = 0.001;
    constexpr int maxIteration = 1000;

    MPI_Init(NULL, NULL);
    int worldRank, worldSize;
    MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

    std::vector<Communication::CentroidsForWorker> allCentroids;

    if (worldRank == 0) // master node
    {
        const auto points = Observation::getData(inputFile);
        Communication::Master::sendInitialPointsToWorkers(points, worldSize);
        allCentroids.resize(worldSize - 1);
        Communication::Master::receiveCentroids(allCentroids, worldSize);

        std::vector<Observation> outputCentroids(k);
        KmeansMPI::mergeCentroids(allCentroids, outputCentroids);
        for (const auto centroid : outputCentroids)
            std::cout << centroid << std::endl;
    }
    else // workers nodes
    {
        auto inputPoints = Communication::Worker::receiveFromMaster();
        std::cout << "Process " << worldRank << " received " << inputPoints.size() << " observations\n";

        std::vector<Observation> centroids(k);
        const auto iterations = KmeansMPI::fit(inputPoints, k, centroids, tolerance, maxIteration);
        Communication::Worker::sendCentroids(centroids);
    }

    MPI_Finalize();
}