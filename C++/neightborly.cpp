#include <vector>
#include <tuple>
#include <iostream>
#include <algorithm>
using namespace std;

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
    sort(vecOfTuples.begin(), vecOfTuples.end(), TupleComparator<Index>(desc));
};

vector<int> get_row_tracks(const vector<tuple<double, double, double>> &matrix)
{
    // Square root of length of matrix must be an integer.
    // And input matrix must also be sorted along the rows.
    int stepSize = static_cast<int>(sqrt(matrix.size()));

    vector<int> result;
    for (int i = 0; i < matrix.size(); i += stepSize)
    {
        result.push_back(get<2>(matrix[i]));
    }

    return result;
};
int number = 0;
// Implementation of Neighborly Algorithm
// Takes costPosArray, setA, setB, sumofDistances
void neighborly(vector<tuple<double, double, double>> &costPosArray, vector<pair<double, double>> setA, vector<pair<double, double>> setB, double sumofDistances = 0)
{
    vector<tuple<double, double, double>> rowSortedCostPosArray = costPosArray;

    sort_by_index<1>(rowSortedCostPosArray);
    size_t size = sqrt(rowSortedCostPosArray.size());

    vector<int> row_track = get_row_tracks(rowSortedCostPosArray);

    vector<int> competition(0);

    for (size_t i = 0; i < row_track.size(); ++i)
    {
        if (get<2>(costPosArray[0]) == row_track[i] && get<1>(costPosArray[0]) != i)
        {
            competition.push_back(i);
        }
    }

    vector<tuple<int, double>> delta = {{get<1>(costPosArray[0]), 1.0}};
    if (size > 1)
    {
        delta = {{get<1>(costPosArray[0]), get<0>(rowSortedCostPosArray[(size * get<1>(costPosArray[0])) + 1]) - get<0>(rowSortedCostPosArray[(size * get<1>(costPosArray[0]))])}};
    }

    for (const auto &item : competition)
    {
        delta.push_back(make_tuple(item, get<0>(rowSortedCostPosArray[(size * item) + 1]) - get<0>(rowSortedCostPosArray[(size * item)])));
    }

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

    setA.erase(setA.begin() + assignmentX);
    setB.erase(setB.begin() + assignmentY);

    // cout << "Length of SetA after erasure: " << setA.size() << endl;
    // cout << "Length of SetB after erasure: " << setB.size() << endl;
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

    cout << "Sum of distances till " << number << " assignments: " << sumofDistances << endl;

    if (newCostPosArray.size() != 0)
    {
        neighborly(newCostPosArray, setA, setB, sumofDistances);
    }
};

int main()
{
    // Given sets
    vector<pair<double, double>> setA = {{3.0, 2.0}, {4.0, 1.0}, {8.0, 5.0}};
    vector<pair<double, double>> setB = {{1.0, 2.0}, {3.0, 6.0}, {1.0, 5.0}};
    int len_A = setA.size();
    int len_B = setB.size();

    // Create a cost matrix
    vector<vector<double>> costMatrixOriginal(len_A, vector<double>(len_B, 0.0));
    for (int i = 0; i < len_A; ++i)
    {
        for (int j = 0; j < len_B; ++j)
        {
            costMatrixOriginal[i][j] = sqrt(pow(setA[i].first - setB[j].first, 2) + pow(setA[i].second - setB[j].second, 2));
        }
    }

    // Cost and Position Array
    vector<tuple<double, double, double>> costPosArray(0);

    for (int i = 0; i < len_A; ++i)
    {
        for (int j = 0; j < len_B; ++j)
        {
            costPosArray.push_back(make_tuple(costMatrixOriginal[i][j], i, j));
        }
    }

    sort_by_index<0>(costPosArray);
    neighborly(costPosArray, setA, setB, 0);

    return 0;
}