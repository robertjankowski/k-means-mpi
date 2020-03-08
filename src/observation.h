#ifndef __OBSERVATION__
#define __OBSERVATION__

#include <vector>

class Point
{
    double x, y;

public:
    Point(double, double);
    double getX() const { return x; }
    double getY() const { return y; }
    void setX(double newX) { x = newX; }
    void setY(double newY) { y = newY; }
    static double distance(const Point &p1, const Point &p2);
};

class Observation : public Point
{
    int clusterId;

public:
    Observation(double, double, int);
    static std::vector<Observation> getData(const std::string &);

    void setClusterId(int id) { clusterId = id; }
    int getClusterId() const { return clusterId; }
    Point getPoint() const { return Point(getX(), getY()); }
};

using ObservationsWithIteration = std::pair<std::vector<Observation>, int>;

#endif // __OBSERVATION__