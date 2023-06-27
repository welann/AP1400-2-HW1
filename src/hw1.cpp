#include "hw1.h"
#include <random>
#include <stdexcept>
#include <vector>

Matrix algebra::zeros(size_t n, size_t m)
{
    Matrix result = Matrix(n, std::vector<double>(m, 0));
    return result;
}
Matrix algebra::ones(size_t n, size_t m)
{
    Matrix result = Matrix(n, std::vector<double>(m, 1));
    return result;
}
Matrix algebra::random(size_t n, size_t m, double min, double max)
{
    if (min > max) {
        throw std::logic_error("min cannot be greater than max");
    }

    Matrix result = Matrix(n, std::vector<double>(m, 0));

    std::random_device dev;
    std::uniform_real_distribution<> dis(min, max);

    for (auto i = 0; i < n; i++) {
        for (auto j = 0; j < m; j++) {
            std::mt19937 rng(dev());
            result[i][j] = dis(rng);
        }
    }

    return result;
}
void algebra::show(const Matrix& matrix)
{
    std::cout << std::fixed << std::setprecision(3);
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[0].size(); j++) {
            std::cout << std::setw(8) << matrix[i][j];
        }
        std::cout << std::endl;
    }
}

Matrix algebra::multiply(const Matrix& matrix, double c)
{
    auto temp = matrix;
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[0].size(); j++) {
            temp[i][j] *= c;
        }
    }
    return temp;
}

Matrix algebra::multiply(const Matrix& matrix1, const Matrix& matrix2)
{
    if (matrix1.empty() || matrix2.empty()) {
        return matrix1.empty() ? matrix1 : matrix2;
    }

    if (matrix1[0].size() != matrix2.size()) {
        throw std::logic_error("matrices with wrong dimensions cannot be multiplied");
    }
    
    Matrix result = Matrix(matrix1.size(), std::vector<double>(matrix2[0].size(), 1));

    for (int i = 0; i < matrix1.size(); i++) {
        for (int j = 0; j < matrix2[0].size(); j++) {
            double temp = 0.0;

            for (int n = 0; n < matrix1[0].size(); n++) {
                temp += matrix1[i][n] * matrix2[n][j];
            }

            result[i][j] = temp;
        }
    }

    return result;
}