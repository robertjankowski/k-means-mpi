#include <iostream>
#include "measure_time.h"
#include "kmeans.h"

int main()
{
    auto points = Observation::getData("../data/n=100000_c=4.csv");

    constexpr int k = 4;
    // const auto centroids = Kmeans::fit(points, k, 0.01, 100);
    // for (auto &c : centroids)
    //     std::cout << c.getX() << " " << c.getY() << '\n';

    auto elapsed_time = measure<milliseconds>::measure_time(Kmeans::fit, points, k, 0.01, 100).count();
    std::cout << "Elapsed: " << elapsed_time  << " ms" << std::endl;
}