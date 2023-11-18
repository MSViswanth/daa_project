#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

// A structure to represent a weighted edge in the bipartite graph
struct Edge {
    int u, v;
    double weight;
};

// A class to represent a bipartite graph for the Hungarian algorithm
class BipartiteGraph {
public:
    int n, m; // Number of vertices in set A and set B
    std::vector<std::vector<double>> weightMatrix;
    std::vector<bool> visitedA, visitedB;  // Added visited arrays

    BipartiteGraph(int n, int m) : n(n), m(m), weightMatrix(n, std::vector<double>(m, 0.0)),
                                    visitedA(n, false), visitedB(m, false) {}

    // Add an edge to the bipartite graph
    void addEdge(int u, int v, double weight) {
        weightMatrix[u][v] = weight;
    }
    // Find a minimum weight matching using the Hungarian algorithm
    std::vector<std::pair<int, int>> hungarianAlgorithm() {
        // Step 1: Subtract the minimum value in each row from all values in that row
        for (int i = 0; i < n; ++i) {
            double minVal = *std::min_element(weightMatrix[i].begin(), weightMatrix[i].end());
            for (int j = 0; j < m; ++j) {
                weightMatrix[i][j] -= minVal;
            }
        }

        // Step 2: Subtract the minimum value in each column from all values in that column
        for (int j = 0; j < m; ++j) {
            double minVal = std::numeric_limits<double>::infinity();
            for (int i = 0; i < n; ++i) {
                minVal = std::min(minVal, weightMatrix[i][j]);
            }
            for (int i = 0; i < n; ++i) {
                weightMatrix[i][j] -= minVal;
            }
        }

        // Step 3: Try to find a complete assignment
        std::vector<int> matchA(n, -1), matchB(m, -1);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                if (weightMatrix[i][j] == 0 && matchB[j] == -1) {
                    matchA[i] = j;
                    matchB[j] = i;
                    visitedA[i] = true;
                    visitedB[j] = true;
                    break;
                }
            }
        }

        // Step 4: Repeatedly try to find an augmenting path
        while (true) {
            bool found = false;
            for (int i = 0; i < n; ++i) {
                if (!visitedA[i]) {
                    visitedB.assign(m, false);  // Reset visitedB
                    if (findAugmentingPath(i, matchA, matchB)) {
                        found = true;
                        break;
                    }
                }
            }
            if (!found) {
                break;
            }
        }

        // Construct the matching pairs
        std::vector<std::pair<int, int>> matching;
        for (int i = 0; i < n; ++i) {
            if (matchA[i] != -1) {
                matching.push_back({i, matchA[i]});
            }
        }

        return matching;
    }

private:
    // Helper function to find an augmenting path
    bool findAugmentingPath(int startA, std::vector<int>& matchA, std::vector<int>& matchB) {
        for (int j = 0; j < m; ++j) {
            if (!visitedB[j] && weightMatrix[startA][j] == 0) {
                visitedB[j] = true;
                if (matchB[j] == -1 || findAugmentingPath(matchB[j], matchA, matchB)) {
                    matchA[startA] = j;
                    matchB[j] = startA;
                    return true;
                }
            }
        }
        return false;
    }
};

// Function to implement the Hungarian algorithm
std::pair<double, std::vector<std::pair<int, int>>> hungarian_algorithm(const std::vector<std::pair<double, double>>& setA,
                                                                       const std::vector<std::pair<double, double>>& setB) {
    int len_A = setA.size();
    int len_B = setB.size();

    // Create a BipartiteGraph
    BipartiteGraph graph(len_A, len_B);

    // Populate the weight matrix in the BipartiteGraph
    for (int i = 0; i < len_A; ++i) {
        for (int j = 0; j < len_B; ++j) {
            double distance = std::sqrt(std::pow(setA[i].first - setB[j].first, 2) + std::pow(setA[i].second - setB[j].second, 2));
            graph.addEdge(i, j, -distance); // Convert distances to weights by negating them
        }
    }

    // Run the Hungarian algorithm
    std::vector<std::pair<int, int>> matching = graph.hungarianAlgorithm();

    // Calculate the sum of minimum distances
    double sum_distances = 0.0;
    for (const auto& pair : matching) {
        sum_distances -= graph.weightMatrix[pair.first][pair.second]; // Negate back to get the distance
    }

    return std::make_pair(sum_distances, matching);
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