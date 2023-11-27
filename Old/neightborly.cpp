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

// Comparator that compares tuples based on the specified index
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

// Function to sort a vector of tuples based on a specified index
template <size_t Index, typename T>
void sort_by_index(vector<T> &vecOfTuples, bool desc = false)
{
    auto start = steady_clock::now();
    sort(vecOfTuples.begin(), vecOfTuples.end(), TupleComparator<Index>(desc));
    auto stop = steady_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by Sort By Index Function: "
         << duration.count() << " microseconds" << endl;
};

vector<int> get_row_tracks(const vector<tuple<double, double, double>> &matrix)
{
    auto start = steady_clock::now();
    // Square root of length of matrix must be an integer.
    // And input matrix must also be sorted along the rows.
    int stepSize = static_cast<int>(sqrt(matrix.size()));

    vector<int> result;

    for (int i = 0; i < matrix.size(); i += stepSize)
    {
        result.push_back(get<2>(matrix[i]));
    }
    auto stop = steady_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by Get Row Tracks Function: "
         << duration.count() << " microseconds" << endl;
    return result;
};
int number = 0;
// Implementation of Neighborly Algorithm
// Takes costPosArray, setA, setB, sumofDistances
void neighborly(vector<tuple<double, double, double>> &costPosArray, vector<pair<double, double>> setA, vector<pair<double, double>> setB, double sumofDistances = 0, steady_clock::time_point start1 = steady_clock::now())
{
    // auto start = steady_clock::now();
    vector<tuple<double, double, double>> rowSortedCostPosArray = costPosArray;
    // auto stop = steady_clock::now();
    // auto duration = duration_cast<microseconds>(stop - start);
    // cout << "Time taken by costPosAssignment: "
    //      << duration.count() << " microseconds" << endl;

    sort_by_index<1>(rowSortedCostPosArray);
    size_t size = sqrt(rowSortedCostPosArray.size());

    vector<int> row_track = get_row_tracks(rowSortedCostPosArray);

    // start = steady_clock::now();
    vector<int> competition(0);

    for (size_t i = 0; i < row_track.size(); ++i)
    {
        if (get<2>(costPosArray[0]) == row_track[i] && get<1>(costPosArray[0]) != i)
        {
            competition.push_back(i);
        }
    }
    // stop = steady_clock::now();
    // duration = duration_cast<microseconds>(stop - start);
    // cout << "Time taken by competitionGeneration: "
    //      << duration.count() << " microseconds" << endl;
    // start = steady_clock::now();
    vector<tuple<int, double>> delta = {{get<1>(costPosArray[0]), 1.0}};
    if (size > 1)
    {
        delta = {{get<1>(costPosArray[0]), get<0>(rowSortedCostPosArray[(size * get<1>(costPosArray[0])) + 1]) - get<0>(rowSortedCostPosArray[(size * get<1>(costPosArray[0]))])}};
    }

    for (const auto &item : competition)
    {
        delta.push_back(make_tuple(item, get<0>(rowSortedCostPosArray[(size * item) + 1]) - get<0>(rowSortedCostPosArray[(size * item)])));
    }
    // stop = steady_clock::now();
    // duration = duration_cast<microseconds>(stop - start);
    // cout << "Time taken by deltaGeneration: "
    //      << duration.count() << " microseconds" << endl;
    sort_by_index<1>(delta, true);

    int assignmentX = get<0>(delta[0]);
    int assignmentY = get<2>(costPosArray[0]);

    // cout<<"AssignmentX: "<<assignmentX<<endl;
    // cout<<"AssignmentY: "<<assignmentY<<endl;

    number++;
    // cout << "Length of SetA before erasure: " << setA.size() << endl;
    // cout << "Length of SetB before erasure: " << setB.size() << endl;

    cout << "Assignment: (" << setA[assignmentX].first << ", " << setA[assignmentX].second << ")"
         << " --> "
         << "(" << setB[assignmentY].first << ", " << setB[assignmentY].second << ")" << endl;
    // start = steady_clock::now();
    setA.erase(setA.begin() + assignmentX);
    setB.erase(setB.begin() + assignmentY);
    // stop = steady_clock::now();
    // duration = duration_cast<microseconds>(stop - start);
    // cout << "Time taken by SetsModification: "
    //      << duration.count() << " microseconds" << endl;
    // cout << "Length of SetA after erasure: " << setA.size() << endl;
    // cout << "Length of SetB after erasure: " << setB.size() << endl;
    // start = steady_clock::now();
    vector<tuple<double, double, double>> newCostPosArray(0);

    for (const auto &element : costPosArray)
    {
        if (get<1>(element) != assignmentX && get<2>(element) != assignmentY)
        {
            newCostPosArray.push_back(element);
        }
        if (get<1>(element) == assignmentX && get<2>(element) == assignmentY)
        {
            cout << "Cost: " << get<0>(element) << endl;
            sumofDistances += get<0>(element);
        }
    }
    // for (size_t i = 0; i < costPosArray.size(); ++i)
    // {
    //     auto &element = costPosArray[i];
    //     if (get<1>(element) == assignmentX && get<2>(element) == assignmentY)
    //     {
    //         cout << "Cost: " << get<0>(element) << endl;
    //         sumofDistances += get<0>(element);
    //     }
    //     if (get<1>(element) == assignmentX || get<2>(element) == assignmentY)
    //     {
    //         // newCostPosArray.push_back(element);
    //         costPosArray.erase(costPosArray.begin() + i);
    //     }
    // }

    // stop = steady_clock::now();
    // duration = duration_cast<microseconds>(stop - start);
    // cout << "Time taken by NewCostArrayGeneration: "
    //      << duration.count() << " microseconds" << endl;
    // start = steady_clock::now();
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
    // stop = steady_clock::now();
    // duration = duration_cast<microseconds>(stop - start);
    // cout << "Time taken by Updating NewCostArray: "
    //      << duration.count() << " microseconds" << endl;

    cout << "Sum of distances till " << number << " assignments: " << sumofDistances << endl;
    auto stop = steady_clock::now();
    auto duration = duration_cast<microseconds>(stop - start1);
    cout << "Time taken by Neigborly iteration: " << number << " time:"
         << duration.count() << " microseconds" << endl;
    start1 = steady_clock::now();
    if (newCostPosArray.size() != 0)
    {

        neighborly(newCostPosArray, setA, setB, sumofDistances, start1);
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
    
    std::string setAfilename = "..\\Dataset\\setA_1000.csv";
    std::string setBfilename = "..\\Dataset\\setB_1000.csv";
    std::vector<std::pair<double, double>> setA = readCSV(setAfilename);
    std::vector<std::pair<double, double>> setB = readCSV(setBfilename);
    
    int len_A = setA.size();
    int len_B = setB.size();
    auto start = steady_clock::now();
    // Create a cost matrix
    vector<vector<double>> costMatrixOriginal(len_A, vector<double>(len_B, 0.0));

    // Cost and Position Array
    vector<tuple<double, double, double>> costPosArray(0);
    for (int i = 0; i < len_A; ++i)
    {
        for (int j = 0; j < len_B; ++j)
        {
            costMatrixOriginal[i][j] = sqrt(pow(setA[i].first - setB[j].first, 2) + pow(setA[i].second - setB[j].second, 2));
            costPosArray.push_back(make_tuple(costMatrixOriginal[i][j], i, j));
        }
    }
    auto stop = steady_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by CostPosGeneration: "
         << duration.count() << " microseconds" << endl;

    sort_by_index<0>(costPosArray);
    start = steady_clock::now();
    neighborly(costPosArray, setA, setB, 0);
    stop = steady_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by Neighborly: "
         << duration.count() << " microseconds" << endl;

    return 0;
}