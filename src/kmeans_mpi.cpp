#include <algorithm>
#include <iostream>
#include <random>
#include <mpi.h>
#include <utility>
#include "kmeans_mpi.h"

void Communication::Master::sendInitialPointsToWorkers(const std::vector<Observation> &points, int worldSize)
{
    // Split input file into `n` worker nodes and send them datapoints
    auto vecOfVecs = Communication::splitVectorByParts<Observation>(points, worldSize - 1);
    for (int i = 0; i < worldSize - 1; ++i)
    {
        auto input = vecOfVecs.at(i);
        MPI_Send(input.data(), input.size() * sizeof(Observation), MPI_BYTE, i + 1, 0, MPI_COMM_WORLD);
    }
}

std::vector<Observation> Communication::Worker::receiveFromMaster()
{
    std::vector<Observation> inputPoints;
    MPI_Status status;
    MPI_Probe(0, 0, MPI_COMM_WORLD, &status);

    int incomingObservationsSize;
    MPI_Get_count(&status, MPI_BYTE, &incomingObservationsSize);
    inputPoints.resize(incomingObservationsSize / sizeof(Observation));

    MPI_Recv(inputPoints.data(), incomingObservationsSize, MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    return inputPoints;
}

double KmeansMPI::costFunction(const std::vector<Observation> &points, const std::vector<Observation> &centroids)
{
    double totalDistance = 0;
    for (const auto &p : points)
        for (const auto &c : centroids)
            totalDistance += Point::distance(p, c);
    return totalDistance;
}

void KmeansMPI::assignToClosestCentroid(Observation &point, const std::vector<Observation> &centroids)
{
    double distance = 10000000;
    for (const auto &centroid : centroids)
    {
        double newDistance = Point::distance(point.getPoint(), centroid.getPoint());
        if (newDistance < distance)
        {
            point.setClusterId(centroid.getClusterId());
            distance = newDistance;
        }
    }
}

void KmeansMPI::updateCentroids(const std::vector<Observation> &initPoints, std::vector<Observation> &centroids)
{
    for (auto &centroid : centroids)
    {
        double x = 0, y = 0;
        int counter = 0;
        for (const auto &p : initPoints)
        {
            if (p.getClusterId() == centroid.getClusterId())
            {
                x += p.getX();
                y += p.getY();
                ++counter;
            }
        }
        if (counter > 0)
        {
            x /= counter;
            y /= counter;
            centroid.setX(x);
            centroid.setY(y);
        }
    }
}

int KmeansMPI::fit(std::vector<Observation> &initPoints, unsigned int k,
                   std::vector<Observation> &centroids, double tolerance, int maxIteration)
{
    std::vector<int> positions(initPoints.size());
    std::iota(positions.begin(), positions.end(), 0);
    std::random_shuffle(positions.begin(), positions.end(), [](const int i) { return rand() % i; });
    for (unsigned int i = 0; i < k; ++i)
    {
        const auto point = initPoints.at(positions.at(i)).getPoint();
        centroids.at(i) = Observation(point.getX(), point.getY(), i);
    }

    for (int i = 0; i < maxIteration; ++i)
    {
        double costOld = costFunction(initPoints, centroids);
        for (auto &point : initPoints)
            assignToClosestCentroid(point, centroids);

        updateCentroids(initPoints, centroids);
        double costNew = costFunction(initPoints, centroids);
        if (std::abs(costOld - costNew) < tolerance)
        {
            std::cout << "Iteration: " << i << '\n';
            return i;
        }
    }
    return maxIteration;
}
