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

// Given sets
// vector<pair<double, double>> setA = {{3.0, 2.0}, {4.0, 1.0}, {8.0, 5.0}};
// vector<pair<double, double>> setB = {{1.0, 2.0}, {3.0, 6.0}, {1.0, 5.0}};
// int len_A = setA.size();
// int len_B = setB.size();

int number = 0;
double sumofDistances = 0;

// Create a cost matrix
vector<vector<double>> costMatrixOriginal = {{19.1, 17.6, 24.7, 19.3, 18.7},
                                             {7.0, 7.3, 1.0, 8.3, 5.3},
                                             {17.2, 13.2, 9.9, 18.6, 13},
                                             {6.6, 4.6, 3.7, 9.1, 2.7},
                                             {23.6, 15.4, 19.8, 25.1, 17.3}};
int n = costMatrixOriginal.size();

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

void neighborly(vector<vector<double>> &costColArray)
{
   
    for(int i = 0; i<costColArray[i].size(); ++i){
        sort_by_index<0>(costColArray[i]);
    }
    
    cout << "Neighborly Executed" << endl;
};

int main()
{

    vector<vector<double>> costColArray(n, {0,0});
     for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            costColArray[i][j] = make_tuple(costMatrixOriginal[i][j], j);
        }
    }

    auto start = steady_clock::now();
    {
        neighborly(costColArray);
    }
    auto stop = steady_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by NeighborlyRecursion: "
         << duration.count() << " microseconds" << endl;
    return 0;
}