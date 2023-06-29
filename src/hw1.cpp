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

Matrix algebra::sum(const Matrix& matrix, double c)
{
    auto result = matrix;
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[0].size(); j++) {
            result[i][j] += c;
        }
    }
    return result;
}

Matrix algebra::sum(const Matrix& matrix1, const Matrix& matrix2)
{
    if (matrix1.size() != matrix2.size()) {
        throw std::logic_error("matrices with wrong dimensions cannot be summed");
    }

    // if (matrix1.empty()) {
    //     if (!matrix2.empty()) {
    //         throw std::logic_error("matrices with wrong dimensions cannot be summed");
    //     }
    //     return matrix2;
    // }
    // if (matrix2.empty()) {
    //     // if (!matrix1.empty()) {
    //     //     throw std::logic_error("matrices with wrong dimensions cannot be summed");
    //     // }
    //     return matrix1;
    // }
    // if (matrix1[0].size() != matrix2[0].size()) {
    //     throw std::logic_error("matrices with wrong dimensions cannot be summed");
    // }

    auto result = matrix1;
    for (int i = 0; i < matrix1.size(); i++) {
        for (int j = 0; j < matrix1[0].size(); j++) {
            result[i][j] += matrix2[i][j];
        }
    }
    return result;
}

Matrix algebra::transpose(const Matrix& matrix)
{
    if (matrix.empty()) {
        return matrix;
    }

    Matrix result(matrix[0].size(), std::vector<double>(matrix.size(), 0.0));

    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[0].size(); j++) {
            result[j][i] += matrix[i][j];
        }
    }

    return result;
}

Matrix algebra::minor(const Matrix& matrix, size_t n, size_t m)
{
    Matrix result(matrix.size() - 1, std::vector<double>(matrix[0].size() - 1, 0.0));

    size_t a = 0;
    for (int i = 0; i < matrix.size(); i++) {
        if (i == n)
            continue;

        size_t b = 0;
        for (int j = 0; j < matrix[0].size(); j++) {
            if (j == m)
                continue;

            result[a][b] = matrix[i][j];
            b++;
        }
        a++;
    }

    return result;
}

double algebra::determinant(const Matrix& matrix)
{
    if (matrix.empty()) {
        return 1;
    }

    if (matrix.size() != matrix[0].size()) {
        throw std::logic_error("non-square matrices have no determinant");
    }

    if (matrix.size() == 2) {
        return matrix[0][0] * matrix[1][1] - matrix[1][0] * matrix[0][1];
    }

    double result = 0.0;
    for (int i = 0; i < matrix.size(); i++) {
        result += matrix[i][0] * determinant(minor(matrix, i, 0)) * pow(-1, i);
    }
    return result;
}

Matrix algebra::inverse(const Matrix& matrix)
{
    if (matrix.empty())
        return matrix;

    if (matrix.size() != matrix[0].size()) {
        throw std::logic_error("non-square matrices have no inverse");
    }
    double det = determinant(matrix);
    if (det == 0) {
        throw std::logic_error("singular matrices have no inverse");
    }

    auto result = matrix;
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[0].size(); j++) {
            // 需要注意的是伴随矩阵[i, j]对应的是[j, i]位置的原始矩阵元素
            result[i][j] = determinant(minor(matrix, j, i)) * pow(-1, i + j);
        }
    }

    return multiply(result, 1 / det);
}

Matrix algebra::concatenate(const Matrix& matrix1, const Matrix& matrix2, int axis)
{
    if (axis == 0 && matrix1[0].size() != matrix2[0].size() || axis == 1 && matrix1.size() != matrix2.size()) {
        throw std::logic_error("matrices with wrong dimensions cannot be concatenated");
    }
    Matrix result;
    if (axis == 0) {
        result.clear();
        result.resize(matrix1.size() + matrix2.size(), std::vector<double>(matrix1[0].size(), 0));

        for (int i = 0; i < matrix1.size(); i++) {
            for (int j = 0; j < matrix1[0].size(); j++) {
                result[i][j] = matrix1[i][j];
            }
        }
        for (int m = 0; m < matrix2.size(); m++) {
            for (int j = 0; j < matrix2[0].size(); j++) {
                result[m + matrix1.size()][j] = matrix2[m][j];
            }
        }
        return result;

    } else {

        result.clear();
        result.resize(matrix1.size(), std::vector<double>(matrix1[0].size() + matrix2[0].size(), 0));

        for (int i = 0; i < matrix1.size(); i++) {
            for (int j = 0; j < matrix1[0].size(); j++) {
                result[i][j] = matrix1[i][j];
            }
        }
        for (int i = 0; i < matrix2.size(); i++) {
            for (int j = 0; j < matrix2[0].size(); j++) {
                result[i][j + matrix1[0].size()] = matrix2[i][j];
            }
        }
        return result;
    }
}