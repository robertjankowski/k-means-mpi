#include "kmeans.h"
#include <fstream>
#include <iostream>
#include <cmath>
#include <sstream>

Point::Point(double x, double y) : x(x), y(y) {}

double Point::distance(const Point &p1, const Point &p2)
{
    const auto x = p1.getX() - p2.getX();
    const auto y = p1.getY() - p2.getY();
    return std::sqrt(x * x + y * y);
}

Observation::Observation(double x, double y, int label, int trueLabel) : Point(x, y), label(label), trueLabel(trueLabel) {}

std::vector<Observation> Observation::getData(const std::string &filename)
{
    std::ifstream file(filename);
    std::vector<Observation> observations;
    std::string line;
    double x, y;
    int trueLabel;
    while (getline(file, line))
    {
        std::stringstream ss(line);
        ss >> x;
        ss.ignore(); // ignore comma
        ss >> y;
        ss.ignore();
        ss >> trueLabel;
        observations.emplace_back(x, y, -1, trueLabel);
    }
    return observations;
}

std::vector<Observation> Kmeans::fit(const std::vector<Observation> &initPoints, unsigned int k, double tolerance, int maxIteration)
{
    // Initialize centroids -> let be the first `k` elements from `initPoints`
    std::vector<Point> centroids;
    for (int i = 0; i < k; ++i)
        centroids.emplace_back(initPoints.at(i).getPoint());

    for (int i = 0; i < maxIteration; ++i)
    {
        for (const auto &point: initPoints) {
            // TODO - maybe some refactoring, more class, see Trello for more info
        }
    }

    return {Observation(1, 1, 1, 1)};
}
