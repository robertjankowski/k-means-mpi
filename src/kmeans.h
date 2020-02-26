#ifndef __K_MEANS__
#define __K_MEANS__

#include <string>
#include <vector>
#include <array>

class Point
{
    double x, y;

public:
    Point(double, double);
    double getX() const { return x; }
    double getY() const { return y; }
    static double distance(const Point &p1, const Point &p2);
};

class Observation : public Point
{
    int label, trueLabel;

public:
    Observation(double, double, int, int);
    static std::vector<Observation> getData(const std::string &);

    int getTrueLabel() const { return trueLabel; }
    int getLabel() const { return label; }
    Point getPoint() const { return Point(getX(), getY()); }
};

class Kmeans
{
public:
    static std::vector<Observation> fit(const std::vector<Observation> &initPoints, unsigned int k, double tolerance = 0.0001, int maxIteration = 500);
};

#endif // __K_MEANS__