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

void neighborly(vector<vector<tuple<double, int>>> &costColArray)
{
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            costColArray[i][j] = make_tuple(costMatrixOriginal[i][j], j);
        }
    }
    // for (int i = 0; i < costMatrixOriginal.size(); ++i)
    // {
    //     for (int j = 0; j < costMatrixOriginal[i].size(); ++j)
    //     {
    //         cout << get<0>(costColArray[i][j]) << ", "<< get<1>(costColArray[i][j])<< endl;
    //     }
    // }
    
    cout << "Neighborly Executed" << endl;
};

int main()
{

    vector<vector<tuple<double, int>>> costColArray(n, vector<tuple<double, int>>(n));

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