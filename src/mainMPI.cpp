#include <iostream>
#include <mpi.h>
#include "utils.h"
#include "observation.h"
#include "kmeans_mpi.h"
#include "kmeans.h"

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

    const auto fileWithK = Utils::loadFilenameWithKClusters(argc, argv);
    const std::string inputFile = std::get<0>(fileWithK);
    const int k = std::get<1>(fileWithK);

    constexpr double tolerance = 0.001;
    constexpr int maxIteration = 1000;

    std::vector<Communication::CentroidsForWorker> allCentroids;
    std::vector<Observation> centroids(k);
    if (worldRank == 0)
        allCentroids.resize(worldSize - 1);

    if (worldRank == 0) // master node
    {
        const auto points = Observation::getData(inputFile);
        Communication::Master::sendInitialPointsToWorkers(points, worldSize);
        Communication::Master::receiveCentroids(allCentroids, worldSize);
    }
    else // workers nodes
    {
        std::vector<Observation> inputPoints = Communication::Worker::receiveFromMaster();
        std::cout << "Process " << worldRank << " received " << inputPoints.size() << " observations\n";

        const auto iterations = KmeansMPI::fit(inputPoints, k, centroids, tolerance, maxIteration);
        std::cout << "Process: " << worldRank << " | elapsed: " << iterations << std::endl;
        Communication::Worker::sendCentroids(centroids);
    }

    std::cout << "rank: " << worldRank << " centroid size: " << centroids.size() << " all: " << allCentroids.size() << std::endl;

    if (worldRank == 0)
    {
        for (const auto centroid : allCentroids)
        {
            std::cout << "Worker no. " << centroid.worker << std::endl;
            for (const auto point : centroid.centroids)
                std::cout << point << std::endl;
            std::cout << std::endl;
        }
    }
    MPI_Finalize();
}