#include <iostream>
#include <vector>
#include <cmath>
#include <limits>

class HungarianAlgorithm {
public:
    HungarianAlgorithm(const std::vector<std::vector<double>>& costMatrix)
        : costMatrix(costMatrix), numRows(costMatrix.size()), numCols(costMatrix[0].size()) {}

    std::pair<double, std::vector<std::pair<int, int>>> run() {
        std::vector<std::vector<double>> costMatrix_copy(numRows, std::vector<double>(numCols, 0.0));
        for (int i = 0; i < numRows; ++i) {
            for (int j = 0; j < numCols; ++j) {
                costMatrix_copy[i][j] = costMatrix[i][j];
                std::cout << i << ", " << j << std::endl;
                std::cout << costMatrix[i][j] << std::endl;
        }
    }
        // Subtract the minimum value in each row from all values in that row
        for (int i = 0; i < numRows; ++i) {
            double minVal = *std::min_element(costMatrix[i].begin(), costMatrix[i].end());
            for (int j = 0; j < numCols; ++j) {
                costMatrix[i][j] -= minVal;
            }
        }

        // Subtract the minimum value in each column from all values in that column
        for (int j = 0; j < numCols; ++j) {
            double minVal = std::numeric_limits<double>::infinity();
            for (int i = 0; i < numRows; ++i) {
                minVal = std::min(minVal, costMatrix[i][j]);
            }
            for (int i = 0; i < numRows; ++i) {
                costMatrix[i][j] -= minVal;
            }
        }

        // Initialize assignment arrays
        std::vector<int> assignment(numRows, -1);
        std::vector<bool> rowCovered(numRows, false);
        std::vector<bool> colCovered(numCols, false);

        // Step 3: Find the matching
        for (int i = 0; i < numRows; ++i) {
            for (int j = 0; j < numCols; ++j) {
                if (costMatrix[i][j] == 0 && assignment[i] == -1 && !colCovered[j]) {
                    assignment[i] = j;
                    colCovered[j] = true;
                    break;
                }
            }
        }

        // Construct the matching pairs
        std::vector<std::pair<int, int>> matching;
        for (int i = 0; i < numRows; ++i) {
            matching.emplace_back(i, assignment[i]);
        }

        // Calculate the sum of minimum distances
        double sumDistances = 0.0;
        for (const auto& pair : matching) {
            sumDistances += std::abs(costMatrix_copy[pair.first][pair.second]);
            //sumDistances += costMatrix_copy[pair.first][pair.second];
            std::cout << sumDistances << std::endl;
        }
        return std::make_pair(sumDistances, matching);
        //return sumDistances, matching;
    }

private:
    std::vector<std::vector<double>> costMatrix;
    int numRows, numCols;
};

// Function to implement the Hungarian algorithm
std::pair<double, std::vector<std::pair<int, int>>> hungarian_algorithm(const std::vector<std::pair<double, double>>& setA,
                                                                       const std::vector<std::pair<double, double>>& setB) {
    int len_A = setA.size();
    int len_B = setB.size();

    // Create a cost matrix
    std::vector<std::vector<double>> costMatrix(len_A, std::vector<double>(len_B, 0.0));

    // Populate the cost matrix
    for (int i = 0; i < len_A; ++i) {
        for (int j = 0; j < len_B; ++j) {
            costMatrix[i][j] = std::sqrt(std::pow(setA[i].first - setB[j].first, 2) + std::pow(setA[i].second - setB[j].second, 2));
            std::cout << costMatrix[i][j] << std::endl;
        }
    }
    // Run the Hungarian algorithm
    HungarianAlgorithm hungarian(costMatrix);
    auto result = hungarian.run();

    return result;
}

int main() {
    // Given sets
    std::vector<std::pair<double, double>> setA = {{3.0, 2.0}, {4.0, 1.0}, {8.0, 5.0}};
    std::vector<std::pair<double, double>> setB = {{1.0, 2.0}, {3.0, 6.0}, {1.0, 5.0}};

    // Run Hungarian algorithm
    auto result = hungarian_algorithm(setA, setB);

    // Output the result
    std::cout << "Sum of minimum distances: " << result.first << std::endl;
    std::cout << "Matching pairs:" << std::endl;
    for (const auto& pair : result.second) {
        std::cout << "(" << pair.first << ", " << pair.second << ")" << std::endl;
    }

    return 0;
}
