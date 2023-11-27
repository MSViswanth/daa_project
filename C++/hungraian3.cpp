
// Step1

#include <iostream>
#include <vector>
#include <cmath>
#include <limits>

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
                std::vector<bool> &colCovered)
{
    const int INF = std::numeric_limits<int>::max() / 2;
    // number of rows
    const size_t n = costMatrix.size();

    // number of columns
    const size_t m = costMatrix[0].size();
    // std::vector<bool> rowMarked(costMatrix.size(), false);
    // std::vector<bool> colMarked(costMatrix.size(), false);
    std::vector<std::vector<int>> starredZeroInRow(n, std::vector<int>(2, -1));
    std::vector<std::vector<int>> starredZeroInCol(m, std::vector<int>(2, -1));
    std::vector<std::vector<int>> primedZeroInRow(n, std::vector<int>(2, -1));
    std::vector<std::vector<int>> primedZeroInCol(m, std::vector<int>(2, -1));

    std::vector<std::vector<int>> starAndPrime(n, std::vector<int>(m, -1));

    // rowCovered = rowCovered1;
    // colCovered = colCovered1;
    bool optimalSolutionFound = false;
    int coveredZeroes = 0;

    // initialize two integer array, with infinity values in them.
    std::vector<double> minRow(n, INF), minCol(m, INF);

    // Step 3 - Wikipedia
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            if (costMatrix[i][j] == 0 && !rowCovered[i] && !colCovered[j])
            {
                rowCovered[i] = true;
                colCovered[i] = true;
                starAndPrime[i][j] = 0;       // Starring Zero.
                starredZeroInRow[i] = {i, j}; // Storing the location of starred zero for a row
                starredZeroInCol[j] = {i, j}; // Storing the location of starred zero for a column
            }
        }
    }

    // Uncover everything
    rowCovered.assign(n, false);
    colCovered.assign(m, false);

    // Step 4 - Wikipedia
    while (!optimalSolutionFound)
    {
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

        // Find non-covered zero and prime it.
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < m; ++j)
            {
                if (costMatrix[i][j] == 0)
                {
                    if (starAndPrime[i][j] == -1)
                    {
                        starAndPrime[i][j] = 1; // Priming Zero.
                        primedZeroInRow[i] = {i, j};
                        primedZeroInCol[j] = {i, j};
                        if (i == starredZeroInRow[i][0])
                        {
                            rowCovered[i] = true;
                            colCovered[starredZeroInRow[i][1]] = false;
                        }
                        else
                        {
                            bool subStepSkip = false;
                            while (!subStepSkip)
                            {
                                // Substep 1
                                if (starredZeroInCol[j][1] == j)
                                {
                                    int rowOfStarredZero = starredZeroInCol[j][0];
                                    // Substep 2
                                    if (primedZeroInRow[rowOfStarredZero][0] != -1)
                                    {
                                        continue; // Loop again to execute Substep 1
                                    }
                                }
                                else
                                {
                                    subStepSkip = true;
                                }
                            }

                        }
                    }
                    else
                    {
                        optimalSolutionFound = true; // Skip to Step 5
                    }
                }
            }
        }

        if (n == costMatrix.size() && n == costMatrix.size()) // Change this
        {
            optimalSolutionFound = true;
        }
        else
        {
            std::cout << "Else condition has run" << std::endl;
            // void adjustMatrix(std::vector<std::vector<double>>& costMatrix,
            //           const std::vector<bool>& rowCovered,
            //           const std::vector<bool>& colCovered) {
            const size_t n = costMatrix.size();
            const size_t m = costMatrix[0].size();
            double minUncovered = std::numeric_limits<double>::max();

            // Find the smallest entry not covered by any line
            for (size_t i = 0; i < n; ++i)
            {
                for (size_t j = 0; j < m; ++j)
                {
                    if (!rowCovered[i] && !colCovered[j] && costMatrix[i][j] < minUncovered)
                    {
                        minUncovered = costMatrix[i][j];
                    }
                }
            }

            // Subtract the smallest entry not covered by any line from the entire matrix
            for (size_t i = 0; i < n; ++i)
            {
                for (size_t j = 0; j < m; ++j)
                {
                    if (!rowCovered[i] && !colCovered[j])
                    {
                        costMatrix[i][j] -= minUncovered;
                    }
                    else if (rowCovered[i] && colCovered[j] && costMatrix[i][j] == 0)
                    {
                        costMatrix[i][j] += minUncovered;
                    }
                }
            }
        }
    }
}
// }

// Step 5

// Step 6

std::vector<int> assignJobs(std::vector<std::vector<double>> &costMatrix,
                            std::vector<bool> &rowCovered,
                            std::vector<bool> &colCovered)
{
    const size_t n = costMatrix.size();
    const size_t m = costMatrix[0].size();
    std::vector<int> assignment(costMatrix.size(), -1);
    std::vector<bool> rowCovered1(costMatrix.size(), false);
    std::vector<bool> colCovered1(costMatrix.size(), false);

    rowCovered = rowCovered1;
    colCovered = colCovered1;

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

int main()
{
    // Example input for setA and setB
    // std::vector<std::pair<double, double>> setA = {{3.0, 2.0}, {4.0, 1.0}, {8.0, 5.0}};
    // std::vector<std::pair<double, double>> setB = {{1.0, 2.0}, {3.0, 6.0}, {1.0, 5.0}};

    // Step 1: Generate cost matrix
    // std::vector<std::vector<double>> costMatrix = generateCostMatrix(setA, setB);

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
    std::vector<std::vector<double>> costMatrix = {
        {0, 108, 0, 150},
        {150, 0, 135, 0},
        {0, 122, 148, 250},
        {0, 4, 5, 6}};

    // Step 2: Row reduction
    // rowReduction(costMatrix);

    // Step 3: Column reduction
    // columnReduction(costMatrix);

    // Initializing row and column coverage vectors
    std::vector<bool> rowCovered(costMatrix.size(), false);
    std::vector<bool> colCovered(costMatrix[0].size(), false);

    // Step 4: Cover zeros
    coverZeros(costMatrix, rowCovered, colCovered);

    // Step 5: Adjust the matrix
    // sadjustMatrix(costMatrix, rowCovered, colCovered);

    // Step 6: Assign jobs to workers
    auto assignment = assignJobs(costMatrix, rowCovered, colCovered);

    std::vector<std::pair<int, int>> matching;
    double sumDistances = 0.0;
    for (int i = 0; i < costMatrix.size(); ++i)
    {
        // if (assignment[i] != -1) {
        matching.emplace_back(i, assignment[i]);
        sumDistances = sumDistances + std::abs(costMatrix[i][assignment[i]]);
        // }
    }

    std::cout << "Sum of minimum distances: " << sumDistances << std::endl;
    std::cout << "Matching pairs:" << std::endl;
    for (const auto &pair : matching)
    {
        std::cout << "(" << pair.first << ", " << pair.second << ")" << std::endl;
    }

    return 0;
}