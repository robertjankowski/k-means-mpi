#include <iostream>
#include "kmeans.h"

int main()
{
    const auto points = Point::getPoints("../data/n=1000_c=2.csv");
    for (auto &p : points)
        std::cout << "x=" << p.getX() << " y=" << p.getY() << " label=" << p.getLabel() << std::endl;
}