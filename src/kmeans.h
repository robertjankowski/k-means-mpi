#ifndef __K_MEANS__
#define __K_MEANS__

#include <utility>
#include "observation.h"

class Kmeans
{
private:
    static double costFunction(const std::vector<Observation> &points, const std::vector<Observation> &centroids);
    static void assignToClosestCentroid(Observation &point, const std::vector<Observation> &centroids);
    static void updateCentroids(const std::vector<Observation> &initPoints, std::vector<Observation> &centroids);

public:
    static ObservationsWithIterations fit(std::vector<Observation> &initPoints, unsigned int k, double tolerance = 0.0001, int maxIteration = 500);
};

#endif // __K_MEANS__