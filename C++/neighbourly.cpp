#include <vector>
#include <tuple>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include "avl_tree.h"
using namespace std;

double sumofDistances = 0;

/**
 * @brief Stores the row element of the assignments.
 */
vector<pair<double, double>> setX(0);

/**
 * @brief Stores the column element of the assignments.
 */
vector<pair<double, double>> setY(0);

/**
 * @brief Implementation of neighbourly algorithm
 */
void neighbourlyalgorithm(vector<pair<double, double>> setA, vector<pair<double, double>> setB)
{
    int len_A = setA.size();
    int len_B = setB.size();
    vector<vector<double>> distanceMatrix(len_A, vector<double>(len_B, 0.0));
    vector<vector<pair<double, int>>> distanceColArray(len_A, vector<pair<double, int>>(len_B, make_pair(0, 0)));
    /**
     * @brief Generating a euclidean distance array with given input sets(setA and setB)
     */
    for (int i = 0; i < len_A; ++i)
    {
        for (int j = 0; j < len_B; ++j)
        {
            distanceMatrix[i][j] = sqrt(pow(setA[i].first - setB[j].first, 2) + pow(setA[i].second - setB[j].second, 2));
            distanceColArray[i][j].first = distanceMatrix[i][j];
            distanceColArray[i][j].second = j;
        }
    }
    double minVal = INFINITY;
    int minValRow = -1;

    int size;
    size = distanceColArray.size();
    int numberOfIterations = size;
    vector<int> competition(0);
    vector<vector<double>> delta(0);
    int assignmentX;
    int assignmentY = -1;

    double minValCurrent;

    AVLTree previousAssignments;

    vector<double> chosenDelta;

    competition.clear();
    delta.clear();

    for (int i = 0; i < distanceColArray.size(); ++i)
    {
        sort(distanceColArray[i].begin(), distanceColArray[i].end());
    }

neighbourly:
    do
    {
        minVal = INFINITY;
        minValRow = -1;

        competition.clear();
        delta.clear();
        minValCurrent = minVal;

        for (int i = 0; i < distanceColArray.size(); ++i)
        {

            minValCurrent = minVal;
            if (previousAssignments.searchTree(distanceColArray[i][0].second))
            {
                distanceColArray[i][0].second = -1;
                distanceColArray[i].emplace_back(distanceColArray[i][0]);

                distanceColArray[i].erase(distanceColArray[i].begin());

                goto neighbourly;
            }
            else
            {
                minVal = min(minVal, distanceColArray[i][0].first);
                if (minVal < minValCurrent)
                {
                    minValRow = i;
                }
            }
        }

        for (int i = 0; i < distanceColArray.size(); ++i)
        {
            if ((distanceColArray[minValRow][0].second == distanceColArray[i][0].second) && distanceColArray[i][0].second != assignmentY)
            {
                competition.push_back(i);
            }
        }

        if (size == 1)
        {
            delta = {{1.0, double(minValRow)}};
        }
        else
        {
            for (auto &competitionRow : competition)
            {
                if (previousAssignments.searchTree(distanceColArray[competitionRow][1].second))
                {
                    distanceColArray[competitionRow][1].second = -1;
                    distanceColArray[competitionRow].emplace_back(distanceColArray[competitionRow][1]);

                    distanceColArray[competitionRow].erase(distanceColArray[competitionRow].begin() + 1);
                    goto neighbourly;
                }
                delta.push_back({{distanceColArray[competitionRow][1].first - distanceColArray[competitionRow][0].first, double(competitionRow)}});
            }
        }
        sort(delta.begin(), delta.end());

        chosenDelta = delta.back();

        assignmentX = chosenDelta[1];

        assignmentY = distanceColArray[assignmentX][0].second;
        previousAssignments.insert(assignmentY);
        numberOfIterations--;

        sumofDistances += distanceColArray[assignmentX][0].first;

        /**
         * @brief Storing the points at positions `assignmentX` and `assignmentY` in `setX` and `setY` respectively
         */

        setX.push_back(setA[assignmentX]);
        setY.push_back(setB[assignmentY]);

        for (int i = 0; i < distanceColArray[assignmentX].size(); ++i)
        {
            distanceColArray[assignmentX][i] = {INFINITY, -1};
        }

    } while (numberOfIterations > 0);
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

    /**
     * @brief Input to the algorithm. Change the file cover_zero_path to desired dataset file cover_zero_path in dataset folder
     */
    std::string setAfilename = "../Dataset/setA/setA_10.csv";
    std::string setBfilename = "../Dataset/setB/setB_10.csv";
    std::vector<std::pair<double, double>> setA = readCSV(setAfilename);
    std::vector<std::pair<double, double>> setB = readCSV(setBfilename);

    auto start = chrono::steady_clock::now();

    /**
     * @brief Invoking neighbourly algorithm
     */
    neighbourlyalgorithm(setA, setB);

    auto stop = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "Time taken by neighbourly algorithm: "
         << duration.count() << " microseconds" << endl;
    cout << "Total minimum distance by neighbourly algorithm: " << sumofDistances << endl;

    /**
     * @brief Snippet to print pair points
     */
    // std::cout << "Matching pairs:" << std::endl;
    // for (int i = 0; i < setX.size(); ++i)
    // {
    //     cout << "(" << setX[i].first << ", " << setX[i].second << ") --> "
    //          << "(" << setY[i].first << ", " << setY[i].second << ")" << endl;
    // }
    return 0;
}