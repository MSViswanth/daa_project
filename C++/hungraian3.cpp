
// Step1

#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <chrono>
#include <vector>
#include <tuple>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>

std::vector<std::vector<double>> generateCostMatrix(const std::vector<std::pair<double, double>> &setA,
                                                    const std::vector<std::pair<double, double>> &setB)
{
    std::vector<std::vector<double>> costMatrix(setA.size(), std::vector<double>(setB.size(), 0.0));

    for (size_t i = 0; i < setA.size(); ++i)
    {
        for (size_t j = 0; j < setB.size(); ++j)
        {
            double distance = std::sqrt(std::pow(setA[i].first - setB[j].first, 2) +
                                        std::pow(setA[i].second - setB[j].second, 2));
            costMatrix[i][j] = distance;
        }
    }

    return costMatrix;
}

// Step 2

void rowReduction(std::vector<std::vector<double>> &costMatrix)
{
    for (size_t i = 0; i < costMatrix.size(); ++i)
    {
        // Find the minimum value in the current row
        double minVal = std::numeric_limits<double>::max();
        for (size_t j = 0; j < costMatrix[i].size(); ++j)
        {
            if (costMatrix[i][j] < minVal)
            {
                minVal = costMatrix[i][j];
            }
        }

        // Subtract the minimum value from each element in the row
        for (size_t j = 0; j < costMatrix[i].size(); ++j)
        {
            costMatrix[i][j] -= minVal;
        }
    }
}

// Step 3

void columnReduction(std::vector<std::vector<double>> &costMatrix)
{
    for (size_t j = 0; j < costMatrix[0].size(); ++j)
    {
        // Find the minimum value in the current column
        double minVal = std::numeric_limits<double>::max();
        for (size_t i = 0; i < costMatrix.size(); ++i)
        {
            if (costMatrix[i][j] < minVal)
            {
                minVal = costMatrix[i][j];
            }
        }

        // Subtract the minimum value from each element in the column
        for (size_t i = 0; i < costMatrix.size(); ++i)
        {
            costMatrix[i][j] -= minVal;
        }
    }
}

// Step 4

void coverZeros(std::vector<std::vector<double>> &costMatrix,
                std::vector<bool> &rowCovered,
                std::vector<bool> &colCovered, std::vector<std::vector<int>> &starAndPrime)
{
    const int INF = std::numeric_limits<int>::max() / 2;

    double minUncovered = INFINITY;

    // number of rows
    const size_t n = costMatrix.size();

    // number of columns
    const size_t m = costMatrix[0].size();

    int numberOfLines = 0;

    bool optimalSolutionFound = false;
    std::vector<std::vector<int>> encounteredStarred(0);
    std::vector<std::vector<int>> encounteredPrime(0);

    std::vector<bool> rowMarked(costMatrix.size(), false);
    std::vector<bool> colMarked(costMatrix.size(), false);

    // Step 3 - Wikipedia
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            if (costMatrix[i][j] == 0 && !rowMarked[i] && !colMarked[j])
            {
                rowMarked[i] = true;
                colMarked[i] = true;
                starAndPrime[i][j] = 0; // Starring Zero.
                // starredZeroInRow[i] = {i, j}; // Storing the location of starred zero for a row
                // starredZeroInCol[j] = {i, j}; // Storing the location of starred zero for a column
            }
        }
    }

    // // Uncover everything
    rowMarked.assign(n, false);
    colMarked.assign(m, false);

    int starredZeroCol = -1; // Column number of starred zero found in row.
    int starredZeroRow = -1; // Row number of starred zero found in col.

    // int primedZeroCol = -1; // Column number of primed zero found in row.
    bool subStepSkip = false;

    std::vector<std::vector<int>> nonCoveredZeroes(0);

    // Step 4 - Wikipedia

step4:
    do
    {
        colCovered.assign(m, false);
        rowCovered.assign(n, false);
        // Cover Columns
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < m; ++j)
            {
                if (starAndPrime[i][j] == 0)
                {
                    colCovered[j] = true;
                }
            }
        }
        // Find non covered zeroes
    findnon:
    {
        nonCoveredZeroes.clear();
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < m; ++j)
            {
                if (costMatrix[i][j] == 0 && !(rowCovered[i] || colCovered[j]))
                {
                    nonCoveredZeroes.push_back({i, j});
                }
            }
        }
        if (nonCoveredZeroes.size() == 0)
        {
            break;
        }

        // prime non covered zero
        if (nonCoveredZeroes.size() != 0)
        {
            for (auto &nonCoveredZero : nonCoveredZeroes)
            {
                int i = nonCoveredZero[0];
                int j = nonCoveredZero[1];

                starAndPrime[i][j] = 1; // Priming Zero.

                // Check for starred zero in row i.
                for (int k = 0; k < m; ++k)
                {
                    if (starAndPrime[i][k] == 0)
                    {
                        starredZeroCol = k;
                    }
                }
                if (starredZeroCol != -1)
                {

                    rowCovered[i] = true;
                    colCovered[starredZeroCol] = false;
                    starredZeroCol = -1;
                    goto findnon;
                }
                else
                {
                    encounteredPrime.push_back({i, j});
                    int currentCol = j;
                substep1:
                    do
                    {

                        // Substep 1
                        for (int row = 0; row < n; ++row)
                        {
                            if (starAndPrime[row][currentCol] == 0)
                            {
                                starredZeroRow = row;
                            }
                        }
                        if (starredZeroRow != -1)
                        {

                            encounteredStarred.push_back({starredZeroRow, currentCol});

                            // Substep 2
                            for (int col = 0; col < m; ++col)
                            {

                                if (starAndPrime[starredZeroRow][col] == 1)
                                {
                                    encounteredPrime.push_back({starredZeroRow, col});
                                    currentCol = col;
                                    starredZeroRow = -1;
                                    goto substep1;
                                }
                            }
                        }
                        else
                        {
                            subStepSkip = true;
                            break;
                        }
                    } while (true);

                    for (auto &encounteredStarredZero : encounteredStarred)
                    {
                        starAndPrime[encounteredStarredZero[0]][encounteredStarredZero[1]] = -1; // Unstarring starred
                    }
                    for (auto &encounteredPrimeZero : encounteredPrime)
                    {
                        starAndPrime[encounteredPrimeZero[0]][encounteredPrimeZero[1]] = 0; // Starring primed
                    }
                    for (int k = 0; k < n; ++k)
                    {
                        for (int l = 0; l < m; ++l)
                        {
                            if (starAndPrime[k][l] == 1)
                            {
                                starAndPrime[k][l] = -1;
                            }
                        }
                    }
                    goto step4;
                }
            }
        }
    }
    } while (true);
    numberOfLines = 0;
    for (int i = 0; i < n; ++i)
    {
        if (rowCovered[i] || colCovered[i])
        {
            numberOfLines++;
        }
    }
    if (numberOfLines != n)
    {
        minUncovered = INFINITY;
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < m; ++j)
            {
                if (!(rowCovered[i] || colCovered[j]))
                {
                    minUncovered = std::min(minUncovered, costMatrix[i][j]);
                }
            }
        }

        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < m; ++j)
            {
                if (!rowCovered[i])
                {
                    costMatrix[i][j] -= minUncovered;
                }
                if (colCovered[j])
                {
                    costMatrix[i][j] += minUncovered;
                }
                if (starAndPrime[i][j] == 1)
                {
                    starAndPrime[i][j] = -1;
                }
            }
        }

        goto step4;
    }
    else if (numberOfLines == n)
    {
        optimalSolutionFound = true;
    }
}
// }

// Step 6

std::vector<int> assignJobs(std::vector<std::vector<double>> &costMatrix,
                            std::vector<bool> &rowCovered,
                            std::vector<bool> &colCovered)
{
    const size_t n = costMatrix.size();
    const size_t m = costMatrix[0].size();
    std::vector<int> assignment(costMatrix.size(), -1);
    // std::vector<bool> rowCovered1(costMatrix.size(), false);
    // std::vector<bool> colCovered1(costMatrix.size(), false);

    // rowCovered = rowCovered1;
    // colCovered = colCovered1;

    while (true)
    {
        bool jobAssigned = false;

        // Step 1: Find a row with only one zero and pair job with worker
        for (size_t i = 0; i < n; ++i)
        {
            int zerosInRow = 0;
            int zeroIndex = -1;

            for (size_t j = 0; j < m; ++j)
            {
                if (costMatrix[i][j] == 0 && !colCovered[j])
                {
                    zerosInRow++;
                    zeroIndex = j;
                }
            }

            if (zerosInRow == 1)
            {
                // Assign job to worker
                colCovered[zeroIndex] = true;
                rowCovered[i] = true;

                // Cross out corresponding row and column
                for (size_t k = 0; k < n; ++k)
                {
                    if (k != i && costMatrix[k][zeroIndex] == 0)
                    {
                        rowCovered[k] = true;
                        assignment[i] = zeroIndex;
                    }
                }
                jobAssigned = true;
                break;
            }
        }

        if (!jobAssigned)
        {
            break;
        }
    }
    return assignment;
}

void print2DArray(std::vector<std::vector<double>> costMatrix)
{
    for (int i = 0; i < costMatrix.size(); ++i)
    {
        for (int j = 0; j < costMatrix[i].size(); ++j)
        {
            std::cout << costMatrix[i][j] << " | ";
        }
        std::cout << "\n";
    }
    std::cout << "\n"
              << std::endl;
}
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
    // Example input for setA and setB
    // std::vector<std::pair<double, double>> setA = {{3.0, 2.0}, {4.0, 1.0}, {8.0, 5.0}};
    // std::vector<std::pair<double, double>> setB = {{1.0, 2.0}, {3.0, 6.0}, {1.0, 5.0}};

    std::string setAfilename = "..\\Dataset\\setA_1000.csv";
    std::string setBfilename = "..\\Dataset\\setB_1000.csv";
    std::vector<std::pair<double, double>> setA = readCSV(setAfilename);
    std::vector<std::pair<double, double>> setB = readCSV(setBfilename);
    auto start = std::chrono::steady_clock::now();
    // Step 1: Generate cost matrix
    std::vector<std::vector<double>> costMatrix = generateCostMatrix(setA, setB);

    // std::vector<std::vector<double>> costMatrix = {
    //     {19.1, 17.6, 24.7, 19.3, 18.7},
    //     {7.0, 7.3, 1.0, 8.3, 5.3},
    //     {17.2, 13.2, 9.9, 18.6, 13},
    //     {6.6, 4.6, 3.7, 9.1, 2.7},
    //     {23.6, 15.4, 19.8, 25.1, 17.3}};
    // https://brilliant.org/wiki/hungarian-matching/
    // std::vector<std::vector<double>> costMatrix = {
    //     {108, 125, 150},
    //     {150, 135, 175},
    //     {122, 148, 250},
    // };

    // std::vector<std::vector<double>> costMatrix = {
    //     {0, 108, 0, 150},
    //     {150, 0, 135, 0},
    //     {0, 122, 148, 250},
    //     {0, 4, 5, 6}};
    std::vector<std::vector<double>> costMatrixOriginal = costMatrix;
    // Step 2: Row reduction
    rowReduction(costMatrix);

    // Step 3: Column reduction
    columnReduction(costMatrix);

    // Initializing row and column coverage vectors
    std::vector<bool> rowCovered(costMatrix.size(), false);
    std::vector<bool> colCovered(costMatrix[0].size(), false);

    // Step 4: Cover zeros
    std::vector<std::vector<int>> starAndPrime(setA.size(), std::vector<int>(setB.size(), -1));
    coverZeros(costMatrix, rowCovered, colCovered, starAndPrime);
    auto assignment = starAndPrime;
    // Step 5: Adjust the matrix
    // sadjustMatrix(costMatrix, rowCovered, colCovered);

    // Step 6: Assign jobs to workers
    // auto assignment = assignJobs(costMatrix, rowCovered, colCovered);

    std::vector<std::pair<int, int>> matching(0);
    double sumDistances = 0.0;
    for (int i = 0; i < costMatrix.size(); ++i)
    {
        for (int j = 0; j < costMatrix[0].size(); ++j)
        {
            // std::cout<<costMatrixOriginal[i][j] << " ";
            if (assignment[i][j] == 0)
            {
                sumDistances += costMatrixOriginal[i][j];
                matching.push_back({i, j});
            }
        }
        // std::cout<<std::endl;
    }
    auto stop = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Time taken by Hungarian Algorithm: "
              << duration.count() << " microseconds" << std::endl;

    std::cout << "Sum of minimum distances: " << sumDistances << std::endl;
    std::cout << "Matching pairs:" << std::endl;
    for (const auto &pair : matching)
    {
        std::cout << "(" << pair.first << ", " << pair.second << ")" << std::endl;
    }

    return 0;
}