#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
using namespace std;
using namespace std::chrono;

class Greedy2
{
private:
    std::vector<std::vector<double>> sorted_cost_array;
    int N;

public:
    Greedy2(std::vector<std::vector<double>> sorted_cost, int size) : sorted_cost_array(sorted_cost), N(size) {}
    //
    std::pair<double, std::vector<std::pair<int, int>>> greedy2()
    {
        double min_distance = 0;
        std::vector<bool> x(N, false);
        std::vector<bool> y(N, false);
        std::vector<std::pair<int, int>> matching;
        int i = 0;
        while (N > 0 && i < sorted_cost_array.size())
        {
            if (!x[sorted_cost_array[i][1]] && !y[sorted_cost_array[i][2]])
            {
                min_distance += sorted_cost_array[i][0];
                x[sorted_cost_array[i][1]] = true;
                y[sorted_cost_array[i][2]] = true;
                matching.emplace_back(sorted_cost_array[i][1], sorted_cost_array[i][2]);
                N--;
            }
            i++;
        }
        return std::make_pair(min_distance, matching);
    }
};

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

void printPoints(const std::vector<std::pair<double, double>> &points)
{
    std::cout << "{";
    for (size_t i = 0; i < points.size(); ++i)
    {
        std::cout << "{" << points[i].first << ", " << points[i].second << "}";
        if (i < points.size() - 1)
        {
            std::cout << ", ";
        }
    }
    std::cout << "}" << std::endl;
}

int main()
{
    // std::vector<std::pair<double, double>> setA = {{3.0, 2.0}, {4.0, 1.0}, {8.0, 5.0}};
    // std::vector<std::pair<double, double>> setB = {{1.0, 2.0}, {3.0, 6.0}, {1.0, 5.0}};
    std::string setAfilename = "/Users/guna/Education/Masters/2ndSemester/COT6405_DAA/daa_project/Dataset/setA_10000.csv";
    std::string setBfilename = "/Users/guna/Education/Masters/2ndSemester/COT6405_DAA/daa_project/Dataset/setB_10000.csv";
    std::vector<std::pair<double, double>> setA = readCSV(setAfilename);
    std::vector<std::pair<double, double>> setB = readCSV(setBfilename);
    int N = setA.size();

    // Generating a cost_array
    std::vector<std::vector<double>> sorted_cost_array;
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            double distance = std::sqrt(std::pow(setA[i].first - setB[j].first, 2) + std::pow(setA[i].second - setB[j].second, 2));
            sorted_cost_array.push_back({distance, static_cast<double>(i), static_cast<double>(j)});
        }
    }

    // Sorting the cost array based on the distances
    std::sort(sorted_cost_array.begin(), sorted_cost_array.end());

    // Creating an instance of Greedy2 and calculating minimum distance
    auto start = high_resolution_clock::now();
    Greedy2 greedy_algo(sorted_cost_array, N);
    auto result = greedy_algo.greedy2();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by Greedy2 Algorithmm: "
         << duration.count() << " microseconds" << endl;
    std::cout << "Minimum Distance: " << result.first << std::endl;
    // std::cout << "Matching pairs:" << std::endl;
    // for (const auto &pair : result.second)
    // {
    //     std::cout << "(" << setA[pair.first].first << ", " << setA[pair.first].second << ") --> "
    //               << "(" << setB[pair.second].first << ", " << setB[pair.second].second << ")" << std::endl;
    // }
    // std::cout << "Total matching points: " << result.second.size() << std::endl;

    return 0;
}
