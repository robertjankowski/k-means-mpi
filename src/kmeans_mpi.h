#ifndef __K_MEANSMPI__
#define __K_MEANSMPI__

#include <utility>
#include "observation.h"

namespace Communication
{

struct CentroidsForWorker
{
    std::vector<Observation> centroids;
    int workerId;
};

namespace Master
{
void sendInitialPointsToWorkers(const std::vector<Observation> &points, int worldSize);

void receiveCentroids(std::vector<CentroidsForWorker> &allCentroids, int worldSize);
} // namespace Master

namespace Worker
{
std::vector<Observation> receiveFromMaster();

void sendCentroids(const std::vector<Observation> &centroids);
} // namespace Worker

template <typename T>
std::vector<std::vector<T>> splitVectorByParts(const std::vector<T> &vec, int n)
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

class KmeansMPI
{
private:
    static double costFunction(const std::vector<Observation> &points, const std::vector<Observation> &centroids);
    static void assignToClosestCentroid(Observation &point, const std::vector<Observation> &centroids);
    static void updateCentroids(const std::vector<Observation> &initPoints, std::vector<Observation> &centroids);
    static void calculateMeanPoint(const std::vector<Point> &points, double &newX, double &newY);
    static void findClosestPoint(Point &p, const Observation &centroid, const std::vector<Observation> &centroids);
public:
    static int fit(std::vector<Observation> &initPoints, unsigned int k,
                   std::vector<Observation> &centroids, double tolerance = 0.0001,
                   int maxIteration = 500);

    static void mergeCentroids(const std::vector<Communication::CentroidsForWorker> &allCentroids, std::vector<Observation> &outputCentroids);
};

#endif // __K_MEANSMPI__