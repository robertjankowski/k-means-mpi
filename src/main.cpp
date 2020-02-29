#include <iostream>
#include "kmeans.h"

int main()
{
    auto points = Observation::getData("../data/n=100000_c=4.csv");

    constexpr int k = 4;
    const auto centroids = Kmeans::fit(points, k, 0.01, 100);
    for (auto &c : centroids)
        std::cout << c.getX() << " " << c.getY() << '\n';
}