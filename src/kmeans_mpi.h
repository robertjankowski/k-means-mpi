#ifndef __K_MEANSMPI__
#define __K_MEANSMPI__

#include <utility>
#include "observation.h"

class KmeansMPI
{
private:
    static double costFunction(const std::vector<Observation> &points, const std::vector<Observation> &centroids);
    static void assignToClosestCentroid(Observation &point, const std::vector<Observation> &centroids);
    static void updateCentroids(const std::vector<Observation> &initPoints, std::vector<Observation> &centroids);

public:
    static int fit(std::vector<Observation> &initPoints, unsigned int k,
                   std::vector<Observation> &centroids, double tolerance = 0.0001,
                   int maxIteration = 500);
};

namespace Communication
{

namespace Master
{
void sendInitialPointsToWorkers(const std::vector<Observation> &points, int worldSize);

}

namespace Worker
{
std::vector<Observation> receiveFromMaster();
}

template <typename T>
std::vector<std::vector<T>> splitByParts(const std::vector<T> &vec, int n)
{
    std::vector<std::vector<T>> output(n);
    int quotient = vec.size() / n;
    int reminder = vec.size() % n;
    int first = 0;
    int last;
    for (int i = 0; i < n; ++i)
    {
        if (i < reminder)
        {
            last = first + quotient + 1;
            output[i] = std::vector<T>(vec.begin() + first, vec.begin() + last);
            first = last;
        }
        else if (i != n - 1)
        {
            last = first + quotient;
            output[i] = std::vector<T>(vec.begin() + first, vec.begin() + last);
            first = last;
        }
        else
            output[i] = std::vector<T>(vec.begin() + first, vec.end());
    }
    return output;
}

} // namespace Communication

#endif // __K_MEANSMPI__