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
               int &step)
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

        step = 2;
    }

    /**
     * @brief Function to clear the covers
     */
    inline void clear_covers(std::vector<int> &cover)
    {
        for (auto &n : cover)
            n = 0;
    }

    
    void coverzeros(std::vector<std::vector<double>> &matrix,
               std::vector<std::vector<double>> &M,
               std::vector<int> &RowCover,
               std::vector<int> &ColCover,
               int &step)
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

        step = 3;
    }

    /**
     * @brief Function to check if the number of rows and columns used to cover the zeros is equal to N. If it is equal then invoke function to find the pair points 
     */
    void validate_lines_coverzero(const std::vector<std::vector<double>> &M,
               std::vector<int> &ColCover,
               int &step)
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
            step = 7;
        }
        else
        {
            step = 4;
        }
    }

    void find_a_zero(int &row,
                     int &col,
                     const std::vector<std::vector<double>> &matrix,
                     const std::vector<int> &RowCover,
                     const std::vector<int> &ColCover)
    {
        int r = 0;
        int c = 0;
        int sz = matrix.size();
        bool done = false;
        row = -1;
        col = -1;

        while (!done)
        {
            c = 0;
            while (true)
            {
                if (matrix[r][c] == 0 && RowCover[r] == 0 && ColCover[c] == 0)
                {
                    row = r;
                    col = c;
                    done = true;
                }
                c += 1;
                if (c >= sz || done)
                    break;
            }
            r += 1;
            if (r >= sz)
                done = true;
        }
    }

    bool star_in_row(int row,
                     const std::vector<std::vector<double>> &M)
    {
        bool tmp = false;
        for (unsigned c = 0; c < M.size(); c++)
            if (M[row][c] == 1)
                tmp = true;

        return tmp;
    }

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
     * @brief Function to check if the number of rows and columns used to cover the zeros is equal to N. If it is equal then invoke function to find the pair points 
     */
    void step4(std::vector<std::vector<double>> &matrix,
               std::vector<std::vector<double>> &M,
               std::vector<int> &RowCover,
               std::vector<int> &ColCover,
               int &path_row_0,
               int &path_col_0,
               int &step)
    {
        int row = -1;
        int col = -1;
        bool done = false;

        while (!done)
        {
            find_a_zero(row, col, matrix, RowCover, ColCover);

            if (row == -1)
            {
                done = true;
                step = 6;
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
                    done = true;
                    step = 5;
                    path_row_0 = row;
                    path_col_0 = col;
                }
            }
        }
    }

    void find_star_in_col(int c,
                          int &r,
                          const std::vector<std::vector<double>> &M)
    {
        r = -1;
        for (unsigned i = 0; i < M.size(); i++)
            if (M[i][c] == 1)
                r = i;
    }

    void find_prime_in_row(int r,
                           int &c,
                           const std::vector<std::vector<double>> &M)
    {
        for (unsigned j = 0; j < M.size(); j++)
            if (M[r][j] == 2)
                c = j;
    }

    void augment_path(std::vector<std::vector<int>> &path,
                      int path_count,
                      std::vector<std::vector<double>> &M)
    {
        for (int p = 0; p < path_count; p++)
            if (M[path[p][0]][path[p][1]] == 1)
                M[path[p][0]][path[p][1]] = 0;
            else
                M[path[p][0]][path[p][1]] = 1;
    }

    void erase_primes(std::vector<std::vector<double>> &M)
    {
        for (auto &row : M)
            for (auto &val : row)
                if (val == 2)
                    val = 0;
    }

    void step5(std::vector<std::vector<int>> &path,
               int path_row_0,
               int path_col_0,
               std::vector<std::vector<double>> &M,
               std::vector<int> &RowCover,
               std::vector<int> &ColCover,
               int &step)
    {
        int r = -1;
        int c = -1;
        int path_count = 1;

        path[path_count - 1][0] = path_row_0;
        path[path_count - 1][1] = path_col_0;

        bool done = false;
        while (!done)
        {
            find_star_in_col(path[path_count - 1][1], r, M);
            if (r > -1)
            {
                path_count += 1;
                path[path_count - 1][0] = r;
                path[path_count - 1][1] = path[path_count - 2][1];
            }
            else
            {
                done = true;
            }

            if (!done)
            {
                find_prime_in_row(path[path_count - 1][0], c, M);
                path_count += 1;
                path[path_count - 1][0] = path[path_count - 2][0];
                path[path_count - 1][1] = c;
            }
        }

        augment_path(path, path_count, M);
        clear_covers(RowCover);
        clear_covers(ColCover);
        erase_primes(M);

        step = 3;
    }

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

    void step6(std::vector<std::vector<double>> &matrix,
               const std::vector<int> &RowCover,
               const std::vector<int> &ColCover,
               int &step)
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

        step = 4;
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

        int path_row_0, path_col_0;

        std::vector<std::vector<int>> path(sz + 1, std::vector<int>(2, 0));

        bool done = false;
        int step = 1;
        while (!done)
        {
            switch (step)
            {
            case 1:
                row_column_reduction(matrix, step);
                break;
            case 2:
                coverzeros(matrix, M, RowCover, ColCover, step);
                break;
            case 3:
                validate_lines_coverzero(M, ColCover, step);
                break;
            case 4:
                step4(matrix, M, RowCover, ColCover, path_row_0, path_col_0, step);
                break;
            case 5:
                step5(path, path_row_0, path_col_0, M, RowCover, ColCover, step);
                break;
            case 6:
                step6(matrix, RowCover, ColCover, step);
                break;
            case 7:
                for (auto &vec : M)
                {
                    vec.resize(original.begin()->size());
                }
                M.resize(original.size());
                done = true;
                break;
            default:
                done = true;
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
    std::cout << "Minimum distance: " << distance << std::endl;
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by Hungarian Algorithm: "
         << duration.count() << " microseconds" << endl;

    return 0;
}