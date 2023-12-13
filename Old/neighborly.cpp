#include <vector>
#include <tuple>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
using namespace std;
using namespace std::chrono;

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

/**
 * @brief Gets the first element in each of the rows of an equivalent matrix. `sort_by_index` on index `1` MUST be called before attempting to get row tracks.
 *
 * @param matrix Takes a row sorted vector of tuples of size `n x n` of a matrix equivalent.
 * @return Row track vector which contains the column numbers of least elements in rows of an equivalent cost matrix.
 */
vector<int> get_row_tracks(const vector<tuple<double, double, double>> &matrix)
{
    int stepSize = static_cast<int>(sqrt(matrix.size()));

    vector<int> result;

    for (int i = 0; i < matrix.size(); i += stepSize)
    {
        result.push_back(get<2>(matrix[i]));
    }

    return result;
};

/**
 * @brief Keeps count of the current iteration for recursion. Useful for debugging.
 */
int number = 0;

/**
 * @brief Stores the total sum of the distances of assignments.
 */
double sumofDistances = 0;

/**
 * @brief Stores the row element of the assignments.
 */
vector<pair<double, double>> setX(0);

/**
 * @brief Stores the column element of the assignments.
 */
vector<pair<double, double>> setY(0);

vector<vector<double>> delta;
/**
 * @brief Implements the neighborly algorithm as mentioned in paper titled "Don't be Greedy, be Neighborly, a new assignment algorithm". Link to the paper - https://ieeexplore.ieee.org/abstract/document/8741571
 *
 * @param[in] costPosArray Takes sorted vector of tuples as input. Each tuple is `(cost[x][y], x, y)` where `cost[x][y]` is the value at `x` and `y` indices in `cost` matrix.
 * @param[in] setA Takes input `setA` as input to keep track of assignments as algorithm progresses.
 * @param[in] setB Takes input `setB` as input to keep track of assignments as algorithm progresses.
 * @return Returns nothing. But it prints the total `sumOfDistances` between the chosen assignments from `costPosArray`, and the assignments themselves.
 */
void neighborly(vector<tuple<double, double, double>> &costPosArray, vector<pair<double, double>> setA, vector<pair<double, double>> setB)
{
    // Debug print
    cout << setA.size() << endl;
    /**
     * @brief Stores a row sorted copy of `costPosArray`.
     */
    vector<tuple<double, double, double>> rowSortedCostPosArray = costPosArray;

    sort_by_index<1>(rowSortedCostPosArray);

    /**
     * Stores the square root of length of input `costPosArray`, which is `n` when `neighborly` is called for the first time. For each recursive call, `size` value decreases by 1.
     */
    size_t size = sqrt(rowSortedCostPosArray.size());

    /**
     * @brief Stores the row_track which is the output from `get_row_tracks`.
     */
    vector<int> row_track = get_row_tracks(rowSortedCostPosArray);

    /**
     * @brief Stores the row indices that may want the same column as competition for current assignment.
     */
    vector<int> competition(0);

    /**
     * @brief Stores a vector of tuples. Each tuple is `(row, delta)` where `row` is the row index of a possible assignment for a specific column, and `delta` is the difference between second and first least elements of the `row`.
     */
    delta = {{
        1.0,
        get<1>(costPosArray[0]),
    }};
    if (size > 1)
    {
        delta = {{get<0>(rowSortedCostPosArray[(size * get<1>(costPosArray[0])) + 1]) - get<0>(rowSortedCostPosArray[(size * get<1>(costPosArray[0]))]), get<1>(costPosArray[0])}};
    }
    for (size_t i = 0; i < row_track.size(); ++i)
    {
        if (get<2>(costPosArray[0]) == row_track[i] && get<1>(costPosArray[0]) != i)
        {
            competition.push_back(i);
            delta.push_back({get<0>(rowSortedCostPosArray[(size * i) + 1]) - get<0>(rowSortedCostPosArray[(size * i)]), double(i)});
        }
    }

    // sort_by_index<1>(delta, true);
    sort(delta.begin(), delta.end());
    int lastIndex = delta.size() - 1;

    /**
     * @brief Stores the row index of the chosen assignment.
     */
    int assignmentX = delta[lastIndex][1];
    /**
     * @brief Stores the column index of the chosen assignment.
     */
    int assignmentY = get<2>(costPosArray[0]);

    number++;

    // Storing the points at positions `assignmentX` and `assignmentY` in `setX` and `setY` respectively.
    setX.push_back(setA[assignmentX]);
    setY.push_back(setB[assignmentY]);

    // cout << "Assignment: (" << setA[assignmentX].first << ", " << setA[assignmentX].second << ")"
    //  << " --> "
    //  << "(" << setB[assignmentY].first << ", " << setB[assignmentY].second << ")" << endl;

    // Erasing the points chosen as the current assignment from `setA` and `setB` respectively.
    setA.erase(setA.begin() + assignmentX);
    setB.erase(setB.begin() + assignmentY);

    /**
     * @brief Stores all the elements from `costPosArray` except for elements in the row and column of the current assignment.
     */
    vector<tuple<double, double, double>> newCostPosArray(0);

    for (const auto &element : costPosArray)
    {
        if (get<1>(element) != assignmentX && get<2>(element) != assignmentY)
        {
            newCostPosArray.push_back(element);
        }
        if (get<1>(element) == assignmentX && get<2>(element) == assignmentY)
        {
            // cout << "Cost: " << get<0>(element) << endl;
            sumofDistances += get<0>(element);
        }
    }

    for (auto &element : newCostPosArray)
    {

        if (get<1>(element) > assignmentX)
        {
            --get<1>(element);
        }
        if (get<2>(element) > assignmentY)
        {
            --get<2>(element);
        }
    }

    // cout << "Sum of distances till " << number << " assignments: " << sumofDistances << endl;

    // Calling `neighborly` function recursively with reduced `newCostPosArray`, `setA`, and `setB`. Recursion exits when size of reduced `newCostPosArray` becomes `0`.
    if (newCostPosArray.size() != 0)
    {
        neighborly(newCostPosArray, setA, setB);
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

int main()
{
    // Given sets
    // vector<pair<double, double>> setA = {{3.0, 2.0}, {4.0, 1.0}, {8.0, 5.0}};
    // vector<pair<double, double>> setB = {{1.0, 2.0}, {3.0, 6.0}, {1.0, 5.0}};
    std::string setAfilename = "../Dataset/setA_10.csv";
    std::string setBfilename = "../Dataset/setA_10.csv";
    std::vector<std::pair<double, double>> setA = readCSV(setAfilename);
    std::vector<std::pair<double, double>> setB = readCSV(setBfilename);
    int len_A = setA.size();
    int len_B = setB.size();

    /**
     * @brief Stores the cost matrix between all the elements in `setA` and `setB`.
     */
    vector<vector<double>> costMatrix(len_A, vector<double>(len_B, 0.0));
    auto start = steady_clock::now();

    /**
     * @brief Stores vector of tuples. Each tuple is `(costMatrix[x][y], x, y)` where `costMatrix[x][y]` is the value at `x` and `y` indices in `costMatrix` matrix.
     */
    vector<tuple<double, double, double>> costPosArray(0);

    // Generating `costMatrix` and `costPosArray`.
    for (int i = 0; i < len_A; ++i)
    {
        for (int j = 0; j < len_B; ++j)
        {
            costMatrix[i][j] = sqrt(pow(setA[i].first - setB[j].first, 2) + pow(setA[i].second - setB[j].second, 2));
            costPosArray.push_back(make_tuple(costMatrix[i][j], i, j));
        }
    }

    sort_by_index<0>(costPosArray);

    neighborly(costPosArray, setA, setB);
    auto stop = steady_clock::now();
    auto duration = duration_cast<seconds>(stop - start);
    cout << "Time taken by Neighborly Recursion: "
         << duration.count() << " Seconds" << endl;
    cout << "Sum of distances: " << sumofDistances << endl;
    for (int i = 0; i < setX.size(); ++i)
    {
        cout << "(" << setX[i].first << ", " << setX[i].second << ") --> "
             << "(" << setY[i].first << ", " << setY[i].second << ")" << endl;
    }
    return 0;
}