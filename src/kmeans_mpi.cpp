#include <algorithm>
#include <iostream>
#include <limits>
#include <random>
#include <mpi.h>
#include <utility>
#include "kmeans_mpi.h"

void Communication::Master::sendInitialPointsToWorkers(const std::vector<Observation> &points, int worldSize)
{
    // Split input file into `n` worker nodes and send them datapoints
    const auto vecOfVecs = Communication::splitVectorByParts<Observation>(points, worldSize - 1);
    for (int i = 0; i < worldSize - 1; ++i)
    {
        const auto input = vecOfVecs.at(i);
        MPI_Send(input.data(), input.size() * sizeof(Observation), MPI_BYTE, i + 1, 0, MPI_COMM_WORLD);
    }
}

void Communication::Master::receiveCentroids(std::vector<CentroidsForWorker> &allCentroids, int worldSize)
{
    if (allCentroids.size() != worldSize - 1)
    {
        std::cerr << "Wrong centroids vector size. Cannot receive centroids from nodes\n";
        return;
    }

    for (int i = 0; i < worldSize - 1; ++i)
    {
        std::vector<Observation> centroid;
        MPI_Status status;
        MPI_Probe(i + 1, 0, MPI_COMM_WORLD, &status);

        int incomingObservationsSize;
        MPI_Get_count(&status, MPI_BYTE, &incomingObservationsSize);
        centroid.resize(incomingObservationsSize / sizeof(Observation));

        MPI_Recv(centroid.data(), incomingObservationsSize, MPI_BYTE, i + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        allCentroids.at(i) = CentroidsForWorker{centroid, i + 1};
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

void Communication::Worker::sendCentroids(const std::vector<Observation> &centroids)
{
    MPI_Send(centroids.data(), centroids.size() * sizeof(Observation), MPI_BYTE, 0, 0, MPI_COMM_WORLD);
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
    double distance = std::numeric_limits<double>::max();
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
    if (centroids.size() != k)
    {
        std::cout << "Kmeans fit failed. Centroids vector should be size: " << k << " got: " << centroids.size() << " \n";
        return -1;
    }
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
            // std::cout << "Iteration: " << i << '\n';
            return i;
        }
    }
    return maxIteration;
}

void KmeansMPI::mergeCentroids(const std::vector<Communication::CentroidsForWorker> &allCentroids, std::vector<Observation> &outputCentroids)
{
    if (outputCentroids.size() != allCentroids.at(0).centroids.size())
    {
        std::cerr << "Wrong centroids sizes for merging\n";
        return;
    }

    // At first initialize outputCentroid from first worker
    for (int i = 0; i < outputCentroids.size(); ++i)
        outputCentroids.at(i) = allCentroids.at(0).centroids.at(i);

    for (auto &centroid : outputCentroids)
    {
        std::vector<Point> newPoints(allCentroids.size());
        for (const auto &centroidWithId : allCentroids)
        {
            Point p;
            findClosestPoint(p, centroid, centroidWithId.centroids);
            const auto X = centroid.getX() + p.getX();
            const auto Y = centroid.getY() + p.getY();
            newPoints.emplace_back(X, Y);
        }
        double newX, newY;
        calculateMeanPoint(newPoints, newX, newY);
        centroid.setX(newX);
        centroid.setY(newY);
    }
}

void KmeansMPI::findClosestPoint(Point &p, const Observation &centroid, const std::vector<Observation> &centroids)
{
    double newDistance = 100000;
    for (const auto &point : centroids)
    {
        const auto distance = Point::distance(centroid.getPoint(), point);
        if (distance < newDistance)
        {
            p = point;
            newDistance = distance;
        }
    }
}

void KmeansMPI::calculateMeanPoint(const std::vector<Point> &points, double &newX, double &newY)
{
    newX = 0.0;
    newY = 0.0;
    for (const auto &point : points)
    {
        newX += point.getX();
        newY += point.getY();
    }
    newX /= points.size();
    newY /= points.size();
}