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

class Kmeans
{
private:
    static double costFunction(const std::vector<Observation> &points, const std::vector<Observation> &centroids);
    static void assignToClosestCentroid(Observation &point, const std::vector<Observation> &centroids);
    static void updateCentroids(const std::vector<Observation> &initPoints, std::vector<Observation> &centroids);

public:
    static std::vector<Observation> fit(std::vector<Observation> &initPoints, unsigned int k, double tolerance = 0.0001, int maxIteration = 500);
};

#endif // __K_MEANS__