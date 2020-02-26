#include <iostream>
#include "kmeans.h"

int main()
{
    const auto points = Observation::getData("../data/n=1000_c=2.csv");
    for (auto &p : points)
        std::cout << "x=" << p.getX() << " y=" << p.getY() << " label=" << p.getTrueLabel() << std::endl;

    constexpr int k = 2;
    const auto res = Kmeans::fit(points, k);
}