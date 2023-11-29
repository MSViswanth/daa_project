#include <vector>
#include <tuple>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
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

/**
 * @struct TupleComparator
 * @brief This is given as one of the inputs for `sort_by_index` function and it helps to modify how sorting happens.
 */
template <size_t Index>
struct TupleComparator
{
    bool reverse;
    TupleComparator(bool i) : reverse(i) {}
    template <typename Tuple>
    bool operator()(const Tuple &tuple1, const Tuple &tuple2) const
    {
        if (reverse)
        {
            return get<Index>(tuple1) > get<Index>(tuple2);
        }
        else
        {
            return get<Index>(tuple1) < get<Index>(tuple2);
        }
    }
};

/**
 *
 * @brief Sorts the given vector of tuples by index of the tuple. Sorts in ascending order by default.
 *
 * @paragraph
 *  The `TupleComparator<Index>` is used as the comparator in the sorting process, allowing you to sort the tuples based on the element at the specified `index (Index)`.
 *  The `desc` parameter determines whether the sorting should be in descending order (`true`) or ascending order (`false`).
 * @param vecOfTuples Vector of tuples. Each tuple is `(cost[x][y], x, y)` where `cost[x][y]` is the value at `x` and `y` indices in `cost` matrix.
 * @param desc defaults to `false` for ascending order.
 * @return Returns nothing. It modifies the input vector.
 */
template <size_t Index, typename T>
void sort_by_index(vector<T> &vecOfTuples, bool desc = false)
{

    sort(vecOfTuples.begin(), vecOfTuples.end(), TupleComparator<Index>(desc));
};
void neighborly(vector<vector<pair<double, int>>> &costColArray, vector<pair<double, double>> setA, vector<pair<double, double>> setB)
// void neighborly(vector<vector<pair<double, int>>> &costColArray)
{
    double minVal = INFINITY;
    int minValRow = -1;

    int size;
    size = costColArray.size();
    int numberOfIterations = size;
    vector<int> competition(0);
    vector<vector<double>> delta(0);
    int assignmentX;
    int assignmentY;

    double minValCurrent;

    vector<double> chosenDelta;
neighborly:
    do
    {
        minVal = INFINITY;
        minValRow = -1;

        competition.clear();
        delta.clear();

        for (int i = 0; i < costColArray.size(); ++i)
        {
            sort(costColArray[i].begin(), costColArray[i].end());
            minValCurrent = minVal;
            minVal = min(minVal, costColArray[i][0].first);
            if (minVal < minValCurrent)
            {
                minValRow = i;
            }
        }

        for (int i = 0; i < costColArray.size(); ++i)
        {
            if (costColArray[minValRow][0].second == costColArray[i][0].second)
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
                delta.push_back({{costColArray[competitionRow][1].first - costColArray[competitionRow][0].first, double(competitionRow)}});
            }
        }
        sort(delta.begin(), delta.end());

        chosenDelta = delta.back();

        assignmentX = chosenDelta[1];

        assignmentY = costColArray[assignmentX][0].second;

        sumofDistances += costColArray[assignmentX][0].first;

        // Storing the points at positions `assignmentX` and `assignmentY` in `setX` and `setY` respectively.
        setX.push_back(setA[assignmentX]);
        setY.push_back(setB[assignmentY]);

        for (auto &innerVector : costColArray)
        {
            for (auto &pairElement : innerVector)
            {
                if (pairElement.second == assignmentY)
                {
                    pairElement.first = INFINITY;
                }
            }
        }
        // Reduce costColArray
        for (int i = 0; i < costColArray[assignmentX].size(); ++i)
        {
            costColArray[assignmentX][i] = {INFINITY, -1};
        }

        // costColArray.erase(costColArray.begin()+minValRow);
        numberOfIterations--;

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
    std::string setAfilename = "/Users/guna/Education/Masters/2ndSemester/COT6405_DAA/daa_project/Dataset/setA_1000.csv";
    std::string setBfilename = "/Users/guna/Education/Masters/2ndSemester/COT6405_DAA/daa_project/Dataset/setB_1000.csv";
    std::vector<std::pair<double, double>> setA = readCSV(setAfilename);
    std::vector<std::pair<double, double>> setB = readCSV(setBfilename);
    int len_A = setA.size();
    int len_B = setB.size();

    vector<vector<double>> costMatrix(len_A, vector<double>(len_B, 0.0));
    auto start = chrono::steady_clock::now();

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

    {
        neighborly(costColArray, setA, setB);
    }
    auto stop = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "Time taken by Neighborly: "
         << duration.count() << " microseconds" << endl;
    cout << "Sum of distances: " << sumofDistances << endl;
    for (int i = 0; i < setX.size(); ++i)
    {
        cout << "(" << setX[i].first << ", " << setX[i].second << ") --> "
             << "(" << setY[i].first << ", " << setY[i].second << ")" << endl;
    }
    return 0;
}