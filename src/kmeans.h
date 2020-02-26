#ifndef __K_MEANS__
#define __K_MEANS__

#include <string>
#include <vector>

class Point
{
    double x, y;
    int label;

public:
    Point(double, double, int);
    static std::vector<Point> getPoints(const std::string &);
    double getX() const { return x; }
    double getY() const { return y; }
    double getLabel() const { return label; } 
};

#endif // __K_MEANS__