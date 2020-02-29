#include "kmeans.h"
#include <fstream>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <sstream>
#include <random>

Point::Point(double x, double y) : x(x), y(y) {}

double Point::distance(const Point &p1, const Point &p2)
{
    const auto x = p1.getX() - p2.getX();
    const auto y = p1.getY() - p2.getY();
    return std::sqrt(x * x + y * y);
}

Observation::Observation(double x, double y, int clusterId) : Point(x, y), clusterId(clusterId) {}

std::vector<Observation> Observation::getData(const std::string &filename)
{
    std::ifstream file(filename);
    std::vector<Observation> observations;
    std::string line;
    double x, y;
    while (getline(file, line))
    {
        std::stringstream ss(line);
        ss >> x;
        ss.ignore(); // ignore comma
        ss >> y;
        observations.emplace_back(x, y, -1);
    }
    return observations;
}

double Kmeans::costFunction(const std::vector<Observation> &points, const std::vector<Observation> &centroids)
{
    double totalDistance = 0;
    for (const auto &p : points)
        for (const auto &c : centroids)
            totalDistance += Point::distance(p, c);
    return totalDistance;
}

void Kmeans::assignToClosestCentroid(Observation &point, const std::vector<Observation> &centroids)
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

void Kmeans::updateCentroids(const std::vector<Observation> &initPoints, std::vector<Observation> &centroids)
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

std::vector<Observation> Kmeans::fit(std::vector<Observation> &initPoints, unsigned int k, double tolerance, int maxIteration)
{
    std::vector<Observation> centroids;
    // random initialization
    std::vector<int> positions(initPoints.size());
    std::iota(positions.begin(), positions.end(), 0);
    std::random_shuffle(positions.begin(), positions.end(), [](const int i) { return rand() % i; });
    for (int i = 0; i < k; ++i)
    {
        const auto point = initPoints.at(positions.at(i)).getPoint();
        centroids.emplace_back(Observation(point.getX(), point.getY(), i));
    }

    for (int i = 0; i < maxIteration; ++i)
    {
        double costOld = costFunction(initPoints, centroids);
        for (auto &point : initPoints)
            assignToClosestCentroid(point, centroids);

        updateCentroids(initPoints, centroids);
        double costNew = costFunction(initPoints, centroids);
        // if (std::abs(costOld - costNew) < tolerance)
        //     break;
    }
    return centroids;
}
