#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <string>
#include <type_traits>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>

namespace HungarianAlgorithm
{
    /**
     * @brief Function to reduce the rows and columns. Find the minimum value in each row and subtract it from all the values in each row and Subtract the minimum value of each column from all the elements in their respective columns
     */
    void row_column_reduction(std::vector<std::vector<double>> &matrix,
                              int &goto_step)
    {
        for (auto &row : matrix)
        {
            auto min_row_value = *std::min_element(begin(row), end(row));
            if (min_row_value > 0)
                for (auto &n : row)
                    n -= min_row_value;
        }

        int sz = matrix.size();
        for (int j = 0; j < sz; ++j)
        {
            double min_col_value = std::numeric_limits<double>::max();
            for (int i = 0; i < sz; ++i)
            {
                min_col_value = std::min(min_col_value, matrix[i][j]);
            }

            if (min_col_value > 0)
            {
                for (int i = 0; i < sz; ++i)
                {
                    matrix[i][j] -= min_col_value;
                }
            }
        }

        goto_step = 2;
    }

    /**
     * @brief Function to clear the covers
     */
    inline void clear_covers(std::vector<int> &cover)
    {
        for (auto &n : cover)
            n = 0;
    }

    /**
     * @brief Function to scan the distance matrix and update the matrix M with 1s in the positions where distance matrix has zero
     */
    void coverzeros(std::vector<std::vector<double>> &matrix,
                    std::vector<std::vector<double>> &M,
                    std::vector<int> &RowCover,
                    std::vector<int> &ColCover,
                    int &goto_step)
    {
        int sz = matrix.size();

        for (int r = 0; r < sz; ++r)
            for (int c = 0; c < sz; ++c)
                if (matrix[r][c] == 0)
                    if (RowCover[r] == 0 && ColCover[c] == 0)
                    {
                        M[r][c] = 1;
                        RowCover[r] = 1;
                        ColCover[c] = 1;
                    }

        clear_covers(RowCover);
        clear_covers(ColCover);

        goto_step = 3;
    }

    /**
     * @brief Function to check if the number of rows and columns used to cover the zeros is equal to N. If it is equal then invoke function to find the pair points
     */
    void validate_lines_coverzero(const std::vector<std::vector<double>> &M,
                                  std::vector<int> &ColCover,
                                  int &goto_step)
    {
        int sz = M.size();
        int colcount = 0;

        for (int r = 0; r < sz; ++r)
            for (int c = 0; c < sz; ++c)
                if (M[r][c] == 1)
                    ColCover[c] = 1;

        for (auto &n : ColCover)
            if (n == 1)
                colcount++;

        if (colcount >= sz)
        {
            goto_step = 7;
        }
        else
        {
            goto_step = 4;
        }
    }
    /**
     * @brief Function to find zeros in distance matrix
     */
    void find_a_zero(int &row,
                     int &col,
                     const std::vector<std::vector<double>> &matrix,
                     const std::vector<int> &RowCover,
                     const std::vector<int> &ColCover)
    {
        int r = 0;
        int c = 0;
        int sz = matrix.size();
        bool optimal_solution_found = false;
        row = -1;
        col = -1;

        while (!optimal_solution_found)
        {
            c = 0;
            while (true)
            {
                if (matrix[r][c] == 0 && RowCover[r] == 0 && ColCover[c] == 0)
                {
                    row = r;
                    col = c;
                    optimal_solution_found = true;
                }
                c += 1;
                if (c >= sz || optimal_solution_found)
                    break;
            }
            r += 1;
            if (r >= sz)
                optimal_solution_found = true;
        }
    }

    /**
     * @brief Function to star zeros in each row
     */
    bool star_in_row(int row,
                     const std::vector<std::vector<double>> &M)
    {
        bool tmp = false;
        for (unsigned c = 0; c < M.size(); c++)
            if (M[row][c] == 1)
                tmp = true;

        return tmp;
    }
    /**
     * @brief Function to find starred zeros in each row
     */
    void find_star_in_row(int row,
                          int &col,
                          const std::vector<std::vector<double>> &M)
    {
        col = -1;
        for (unsigned c = 0; c < M.size(); c++)
            if (M[row][c] == 1)
                col = c;
    }

    /**
     * @brief Function to find the zero and identify cover_zero_path to cover each zero
     */
    void find_cover_zero_path(std::vector<std::vector<double>> &matrix,
                              std::vector<std::vector<double>> &M,
                              std::vector<int> &RowCover,
                              std::vector<int> &ColCover,
                              int &cover_zero_path_row_0,
                              int &cover_zero_path_col_0,
                              int &goto_step)
    {
        int row = -1;
        int col = -1;
        bool optimal_solution_found = false;

        while (!optimal_solution_found)
        {
            find_a_zero(row, col, matrix, RowCover, ColCover);

            if (row == -1)
            {
                optimal_solution_found = true;
                goto_step = 6;
            }
            else
            {
                M[row][col] = 2;
                if (star_in_row(row, M))
                {
                    find_star_in_row(row, col, M);
                    RowCover[row] = 1;
                    ColCover[col] = 0;
                }
                else
                {
                    optimal_solution_found = true;
                    goto_step = 5;
                    cover_zero_path_row_0 = row;
                    cover_zero_path_col_0 = col;
                }
            }
        }
    }

    /**
     * @brief Function to find starred zeros in each columns
     */
    void find_star_in_col(int c,
                          int &r,
                          const std::vector<std::vector<double>> &M)
    {
        r = -1;
        for (unsigned i = 0; i < M.size(); i++)
            if (M[i][c] == 1)
                r = i;
    }

    /**
     * @brief Function to find prime zeros in each rows
     */
    void find_prime_in_row(int r,
                           int &c,
                           const std::vector<std::vector<double>> &M)
    {
        for (unsigned j = 0; j < M.size(); j++)
            if (M[r][j] == 2)
                c = j;
    }

    /**
     * @brief Function to agument cover_zero_path that covers zeros
     */

    void augment_cover_zero_path(std::vector<std::vector<int>> &cover_zero_path,
                                 int cover_zero_path_count,
                                 std::vector<std::vector<double>> &M)
    {
        for (int p = 0; p < cover_zero_path_count; p++)
            if (M[cover_zero_path[p][0]][cover_zero_path[p][1]] == 1)
                M[cover_zero_path[p][0]][cover_zero_path[p][1]] = 0;
            else
                M[cover_zero_path[p][0]][cover_zero_path[p][1]] = 1;
    }

    /**
     * @brief Function to clear the prime
     */
    void clear_primes(std::vector<std::vector<double>> &M)
    {
        for (auto &row : M)
            for (auto &val : row)
                if (val == 2)
                    val = 0;
    }

    /**
     * @brief Function to find star and prime and construct cover_zero_path
     */
    void construct_cover_zero_path(std::vector<std::vector<int>> &cover_zero_path,
                                   int cover_zero_path_row_0,
                                   int cover_zero_path_col_0,
                                   std::vector<std::vector<double>> &M,
                                   std::vector<int> &RowCover,
                                   std::vector<int> &ColCover,
                                   int &goto_step)
    {
        int r = -1;
        int c = -1;
        int cover_zero_path_count = 1;

        cover_zero_path[cover_zero_path_count - 1][0] = cover_zero_path_row_0;
        cover_zero_path[cover_zero_path_count - 1][1] = cover_zero_path_col_0;

        bool optimal_solution_found = false;
        while (!optimal_solution_found)
        {
            find_star_in_col(cover_zero_path[cover_zero_path_count - 1][1], r, M);
            if (r > -1)
            {
                cover_zero_path_count += 1;
                cover_zero_path[cover_zero_path_count - 1][0] = r;
                cover_zero_path[cover_zero_path_count - 1][1] = cover_zero_path[cover_zero_path_count - 2][1];
            }
            else
            {
                optimal_solution_found = true;
            }

            if (!optimal_solution_found)
            {
                find_prime_in_row(cover_zero_path[cover_zero_path_count - 1][0], c, M);
                cover_zero_path_count += 1;
                cover_zero_path[cover_zero_path_count - 1][0] = cover_zero_path[cover_zero_path_count - 2][0];
                cover_zero_path[cover_zero_path_count - 1][1] = c;
            }
        }

        augment_cover_zero_path(cover_zero_path, cover_zero_path_count, M);
        clear_covers(RowCover);
        clear_covers(ColCover);
        clear_primes(M);

        goto_step = 3;
    }

    /**
     * @brief Function to identify minimum value in uncovered rows and columns
     */
    void find_min_row_value(double &min_col_value,
                            const std::vector<std::vector<double>> &matrix,
                            const std::vector<int> &RowCover,
                            const std::vector<int> &ColCover)
    {
        for (unsigned r = 0; r < matrix.size(); r++)
            for (unsigned c = 0; c < matrix.size(); c++)
                if (RowCover[r] == 0 && ColCover[c] == 0)
                    if (min_col_value > matrix[r][c])
                        min_col_value = matrix[r][c];
    }

    /**
     * @brief Function to adjust the matrix by subtracting the minimum value from uncovered rows and columns from each uncovered rows and add it to covered columns
     */
    void adjust_matrix(std::vector<std::vector<double>> &matrix,
                       const std::vector<int> &RowCover,
                       const std::vector<int> &ColCover,
                       int &goto_step)
    {
        double min_col_value = std::numeric_limits<double>::max();
        find_min_row_value(min_col_value, matrix, RowCover, ColCover);

        int sz = matrix.size();
        for (int r = 0; r < sz; r++)
            for (int c = 0; c < sz; c++)
            {
                if (RowCover[r] == 1)
                    matrix[r][c] += min_col_value;
                if (ColCover[c] == 0)
                    matrix[r][c] -= min_col_value;
            }

        goto_step = 4;
    }

    double hungarian(std::vector<std::vector<double>> &original)
    {
        std::vector<std::vector<double>> matrix(original.size(),
                                                std::vector<double>(original.begin()->size()));

        auto it = original.begin();
        for (auto &vec : matrix)
        {
            std::copy(it->begin(), it->end(), vec.begin());
            it = std::next(it);
        }

        std::size_t sz = matrix.size();

        std::vector<std::vector<double>> M(sz, std::vector<double>(sz, 0));

        std::vector<int> RowCover(sz, 0);
        std::vector<int> ColCover(sz, 0);

        int cover_zero_path_row_0, cover_zero_path_col_0;

        std::vector<std::vector<int>> cover_zero_path(sz + 1, std::vector<int>(2, 0));

        bool optimal_solution_found = false;
        int goto_step = 1;
        while (!optimal_solution_found)
        {
            switch (goto_step)
            {
            case 1:
                row_column_reduction(matrix, goto_step);
                break;
            case 2:
                coverzeros(matrix, M, RowCover, ColCover, goto_step);
                break;
            case 3:
                validate_lines_coverzero(M, ColCover, goto_step);
                break;
            case 4:
                find_cover_zero_path(matrix, M, RowCover, ColCover, cover_zero_path_row_0, cover_zero_path_col_0, goto_step);
                break;
            case 5:
                construct_cover_zero_path(cover_zero_path, cover_zero_path_row_0, cover_zero_path_col_0, M, RowCover, ColCover, goto_step);
                break;
            case 6:
                adjust_matrix(matrix, RowCover, ColCover, goto_step);
                break;
            case 7:
                for (auto &vec : M)
                {
                    vec.resize(original.begin()->size());
                }
                M.resize(original.size());
                optimal_solution_found = true;
                break;
            default:
                optimal_solution_found = true;
                break;
            }
        }

        {
            double minimal_distance = 0.0;

            for (unsigned j = 0; j < original.begin()->size(); ++j)
                for (unsigned i = 0; i < original.size(); ++i)
                    if (M[i][j])
                    {
                        auto it1 = original.begin();
                        std::advance(it1, i);
                        auto it2 = it1->begin();
                        std::advance(it2, j);
                        minimal_distance += *it2;
                        continue;
                    }

            return minimal_distance;
        }
    }

    /**
     * @brief Function to read data from csv file
     */
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
}

int main()
{
    using namespace HungarianAlgorithm;
    using namespace std;
    using namespace std::chrono;

    /**
     * @brief Input to the algorithm. Change the file cover_zero_path to desired dataset file cover_zero_path in dataset folder
     */
    std::string setAfilename = "../Dataset/setA_10.csv";
    std::string setBfilename = "../Dataset/setB_10.csv";
    std::vector<std::pair<double, double>> setA = readCSV(setAfilename);
    std::vector<std::pair<double, double>> setB = readCSV(setBfilename);

    auto start = std::chrono::steady_clock::now();

    /**
     * @brief Generate distance matrix using euclidean distance
     */
    std::vector<std::vector<double>> distanceMatrix(setA.size(), std::vector<double>(setB.size(), 0.0));
    for (size_t i = 0; i < setA.size(); ++i)
    {
        for (size_t j = 0; j < setB.size(); ++j)
        {
            double distance = std::sqrt(std::pow(setA[i].first - setB[j].first, 2) +
                                        std::pow(setA[i].second - setB[j].second, 2));
            distanceMatrix[i][j] = distance;
        }
    }

    /**
     * @brief Invoke hungarian algorithm with distance matrix as input. It is minimum distance between the pair points as output
     */
    double distance = hungarian(distanceMatrix);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by hungarian algorithm: "
         << duration.count() << " microseconds" << endl;
    std::cout << "Total minimum distance by hungarian algorithm: " << distance << std::endl;

    return 0;
}