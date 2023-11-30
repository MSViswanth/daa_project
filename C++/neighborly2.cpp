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

// Given sets
// vector<pair<double, double>> setA = {{3.0, 2.0}, {4.0, 1.0}, {8.0, 5.0}};
// vector<pair<double, double>> setB = {{1.0, 2.0}, {3.0, 6.0}, {1.0, 5.0}};
// int len_A = setA.size();
// int len_B = setB.size();

double sumofDistances = 0;

/**
 * @brief Stores the row element of the assignments.
 */
vector<pair<double, double>> setX(0);

/**
 * @brief Stores the column element of the assignments.
 */
vector<pair<double, double>> setY(0);

// Create a cost matrix
// vector<vector<double>> costMatrixOriginal = {{19.1, 17.6, 24.7, 19.3, 18.7},
//                                              {7.0, 7.3, 1.0, 8.3, 5.3},
//                                              {17.2, 13.2, 9.9, 18.6, 13},
//                                              {6.6, 4.6, 3.7, 9.1, 2.7},
//                                              {23.6, 15.4, 19.8, 25.1, 17.3}};
// int n = costMatrixOriginal.size();

void neighborly(vector<pair<double, double>> setA, vector<pair<double, double>> setB)
{
    int len_A = setA.size();
    int len_B = setB.size();
    vector<vector<double>> costMatrix(len_A, vector<double>(len_B, 0.0));
    vector<vector<pair<double, int>>> costColArray(len_A, vector<pair<double, int>>(len_B, make_pair(0, 0)));
    for (int i = 0; i < len_A; ++i)
    {
        for (int j = 0; j < len_B; ++j)
        {
            costMatrix[i][j] = sqrt(pow(setA[i].first - setB[j].first, 2) + pow(setA[i].second - setB[j].second, 2));
            costColArray[i][j].first = costMatrix[i][j];
            costColArray[i][j].second = j;
        }
    }
    double minVal = INFINITY;
    int minValRow = -1;

    int size;
    size = costColArray.size();
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

    for (int i = 0; i < costColArray.size(); ++i)
    {
        sort(costColArray[i].begin(), costColArray[i].end());
    }

neighborly:
    do
    {
        minVal = INFINITY;
        minValRow = -1;

        competition.clear();
        delta.clear();
        minValCurrent = minVal;

        for (int i = 0; i < costColArray.size(); ++i)
        {

            minValCurrent = minVal;
            if (previousAssignments.searchTree(costColArray[i][0].second))
            // if (costColArray[i][0].second == assignmentY)
            {
                costColArray[i][0].second = -1;
                costColArray[i].emplace_back(costColArray[i][0]);

                costColArray[i].erase(costColArray[i].begin());

                goto neighborly;
            }
            else
            {
                minVal = min(minVal, costColArray[i][0].first);
                if (minVal < minValCurrent)
                {
                    minValRow = i;
                }
            }
        }

        for (int i = 0; i < costColArray.size(); ++i)
        {
            if ((costColArray[minValRow][0].second == costColArray[i][0].second) && costColArray[i][0].second != assignmentY)
            {
                // cout << i;
                // cout << "Finish the rest after";
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
                if (previousAssignments.searchTree(costColArray[competitionRow][1].second))
                {
                    costColArray[competitionRow][1].second = -1;
                    costColArray[competitionRow].emplace_back(costColArray[competitionRow][1]);

                    costColArray[competitionRow].erase(costColArray[competitionRow].begin()+1);
                    goto neighborly;
                }
                delta.push_back({{costColArray[competitionRow][1].first - costColArray[competitionRow][0].first, double(competitionRow)}});
            }
        }
        sort(delta.begin(), delta.end());

        chosenDelta = delta.back();

        assignmentX = chosenDelta[1];

        assignmentY = costColArray[assignmentX][0].second;
        previousAssignments.insert(assignmentY);
        numberOfIterations--;

        sumofDistances += costColArray[assignmentX][0].first;

        // Storing the points at positions `assignmentX` and `assignmentY` in `setX` and `setY` respectively.
        setX.push_back(setA[assignmentX]);
        setY.push_back(setB[assignmentY]);

        // for (auto &innerVector : costColArray)
        // {
        //     for (auto &pairElement : innerVector)
        //     {
        //         if (pairElement.second == assignmentY)
        //         {
        //             pairElement.first = INFINITY;
        //         }
        //     }
        // }
        // Reduce costColArray
        for (int i = 0; i < costColArray[assignmentX].size(); ++i)
        {
            costColArray[assignmentX][i] = {INFINITY, -1};
        }

        // costColArray.erase(costColArray.begin()+minValRow);

        // cout<<numberOfIterations<<endl;

    } while (numberOfIterations > 0);
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

int main()
{
    // Given sets
    // vector<pair<double, double>> setA = {{3.0, 2.0}, {4.0, 1.0}, {8.0, 5.0}};
    // vector<pair<double, double>> setB = {{1.0, 2.0}, {3.0, 6.0}, {1.0, 5.0}};
    vector<pair<double, double>> setA = {{1.2, 2.4}, {2.3, 3.6}, {5.6, 7.3}};
    vector<pair<double, double>> setB = {{7.9, 3.1}, {2.7, 3.2}, {2.3, 7.4}};
    // std::string setAfilename = "../Dataset/setA_10000.csv";
    // std::string setBfilename = "../Dataset/setB_10000.csv";
    // std::vector<std::pair<double, double>> setA = readCSV(setAfilename);
    // std::vector<std::pair<double, double>> setB = readCSV(setBfilename);

    auto start = chrono::steady_clock::now();

    // Calling Neighborly Implementation.
    neighborly(setA, setB);

    auto stop = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "Time taken by Neighborly: "
         << duration.count() << " microseconds" << endl;
    cout << "Sum of distances: " << sumofDistances << endl;
    // for (int i = 0; i < setX.size(); ++i)
    // {
    //     cout << "(" << setX[i].first << ", " << setX[i].second << ") --> "
    //          << "(" << setY[i].first << ", " << setY[i].second << ")" << endl;
    // }
    return 0;
}