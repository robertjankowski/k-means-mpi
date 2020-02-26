#include "kmeans.h"
#include <fstream>
#include <iostream>
#include <sstream>

Point::Point(double x, double y, int label) : x(x), y(y), label(label) {}

std::vector<Point> Point::getPoints(const std::string &filename)
{
    std::ifstream file(filename);
    std::vector<Point> points;
    std::string line;
    double x, y;
    int label;
    while (getline(file, line))
    {
        std::stringstream ss(line);
        ss >> x;
        ss.ignore(); // ignore comma
        ss >> y;
        ss.ignore();
        ss >> label;
        points.emplace_back(x, y, label);
    }
    return points;
}
