#include <cmath>
#include <fstream>
#include <string>
#include <sstream>
#include "observation.h"

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
