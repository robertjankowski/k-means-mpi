#ifndef __UTILS__
#define __UTILS__

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <utility>

namespace Utils
{

std::vector<std::string> split(const std::string &in, char delim)
{
    std::istringstream ss(in);
    std::string token;
    std::vector<std::string> out;
    while (std::getline(ss, token, delim))
        out.push_back(token);
    return out;
}

std::pair<std::string, int> loadFilenameWithKClusters(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <data_filename> n_clusters\n";
        exit(1);
    }
    const std::string inputFile = argv[1];
    int k = atoi(argv[2]);
    return std::make_pair(inputFile, k);
}

} // namespace Utils

#endif // __UTILS__