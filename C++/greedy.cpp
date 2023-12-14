#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include "avl_tree.h"
using namespace std;
using namespace std::chrono;

class GreedyAlgorithm
{
private:
    std::vector<std::vector<double>> sorted_distance_array;
    int N;

public:
    /**
     * @brief Implementation of greedy algorithm
     */
    GreedyAlgorithm(std::vector<std::vector<double>> sorted_distance, int size) : sorted_distance_array(sorted_distance), N(size) {}
    std::pair<double, std::vector<std::pair<int, int>>> greedy()
    {
        double min_distance = 0;
        AVLTree x;
        AVLTree y;
        std::vector<std::pair<int, int>> matching;
        int i = 0;
        while (N > 0 && i < sorted_distance_array.size())
        {
            if (!x.searchTree(sorted_distance_array[i][1]) && !y.searchTree(sorted_distance_array[i][2]))
            {
                min_distance += sorted_distance_array[i][0];
                x.insert(sorted_distance_array[i][1]);
                y.insert(sorted_distance_array[i][2]);
                matching.emplace_back(sorted_distance_array[i][1], sorted_distance_array[i][2]);
                N--;
            }
            i++;
        }
        return std::make_pair(min_distance, matching);
    }
};

/**
 * @brief Function to read data from csv file
 */
std::vector<std::pair<double, double>> readCSV(const std::string &filename)
{
    std::ifstream file(filename);
    std::vector<std::pair<double, double>> points;

    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            std::stringstream linestream(line);
            std::string value;
            std::vector<double> row;

            while (std::getline(linestream, value, ','))
            {
                row.push_back(std::stod(value));
            }

            if (row.size() == 2)
            {
                points.emplace_back(row[0], row[1]);
            }
        }
        file.close();
    }

    return points;
}

int main()
{

    std::string setAfilename = "../Dataset/setA/setA_10.csv";
    std::string setBfilename = "../Dataset/setB/setB_10.csv";
    std::vector<std::pair<double, double>> setA = readCSV(setAfilename);
    std::vector<std::pair<double, double>> setB = readCSV(setBfilename);
    int N = setA.size();

    auto start = high_resolution_clock::now();

    /**
     * @brief Generating a euclidean distance array with given input sets(setA and setB)
     */
    std::vector<std::vector<double>> sorted_distance_array;
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            double distance = std::sqrt(std::pow(setA[i].first - setB[j].first, 2) + std::pow(setA[i].second - setB[j].second, 2));
            sorted_distance_array.push_back({distance, static_cast<double>(i), static_cast<double>(j)});
        }
    }
    /**
     * @brief Sorting the distance array based on the distances using inbuit merge sort
     */
    std::sort(sorted_distance_array.begin(), sorted_distance_array.end());

    //
    /**
     * @brief Creating an instance of GreedyAlgorithm to invoke greedy function
     */
    GreedyAlgorithm greedy_algo(sorted_distance_array, N);
    auto result = greedy_algo.greedy();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by greedy algorithm: "
         << duration.count() << " microseconds" << endl;
    std::cout << "Total minimum distance by greedy algorithm: " << result.first << std::endl;

    /**
     * @brief Snippet to print pair points
     */
    // std::cout << "Matching pairs:" << std::endl;
    // for (const auto &pair : result.second)
    // {
    //     std::cout << "(" << setA[pair.first].first << ", " << setA[pair.first].second << ") --> "
    //               << "(" << setB[pair.second].first << ", " << setB[pair.second].second << ") --> " << std::sqrt(std::pow(setA[pair.first].first - setB[pair.first].first, 2) + std::pow(setA[pair.second].second - setB[pair.second].second, 2)) << std::endl;
    // }

    return 0;
}
