#include <iostream>
#include <vector>
#include <cmath>
#include <stdio.h>

std::vector<std::vector<double>>Cost_matrix(const std::vector<std::pair<double, double>>& setA,
                                                                       const std::vector<std::pair<double, double>>& setB) {
    
    int len_A = setA.size();
    int len_B = setB.size();

    // Create and initialize the cost matrix
    std::vector<std::vector<double>> AB_matrix(len_A, std::vector<double>(len_B, 0.0));

    // Compute the cost matrix
    for (int i = 0; i < len_A; ++i) {
        for (int j = 0; j < len_B; ++j) {
            AB_matrix[i][j] = std::sqrt(std::pow(setA[i].first - setB[j].first, 2) + std::pow(setA[i].second - setB[j].second, 2));
        }
    }
    return AB_matrix;
                                                                       }
    int main() {
    // Given sets
    std::vector<std::pair<double, double>> setA = {{3.0, 2.0}, {4.0, 1.0}, {8.0, 5.0}};
    std::vector<std::pair<double, double>> setB = {{1.0, 2.0}, {3.0, 6.0}, {1.0, 5.0}};

    // Run Hungarian algorithm
    auto matrix = Cost_matrix(setA, setB);

    // Output the result
    std::cout << "Cost Matrix: " << matrix[1][1] << std::endl;

    return 0;
}