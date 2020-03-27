#include "Matrix.h"
#include <iostream>
#include <cmath>


Matrix::Matrix(std::string file_path, int number, std::string str) {
    std::ifstream file(file_path);
    if (!file.is_open())
        throw std::runtime_error("path error");

    /* считывание всего файла в строку */
    String string;
    while (!file.eof())
        string += file.get();

    /* разделение на отдельные матрицы (через пустую строку) */
    std::vector<String> variants = string.strip().split("\n\n");
    if (number > variants.size() - 1)
        throw std::runtime_error("matrix number error");

    /* разделение на строки с отдельными числами (через 4 пробела)*/
    std::vector<std::vector<double>> vector;
    std::vector<String> lines = variants[number].strip().split("\n");
    if (lines.size() < 2)
        throw std::runtime_error("matrix error");

    for (String line: lines) {
        std::vector<double> numbers;
        for (const String& number_string: line.strip().split(str)) {
            if (number_string.empty()) // проверка на пустое число при повторении разделителя
                continue;
            numbers.push_back(std::strtod(number_string.data(), nullptr));
        }
        if (numbers.size() == 0) // проверка на пустую строку при повторении разделителя \n
            continue;
        vector.push_back(numbers);
    }
    this->setMatrix(vector);
} //

Matrix::Matrix(std::vector<std::vector<double>> vector) {
    this->setMatrix(vector);
}

Matrix::Matrix(int lines, int columns) {
    std::vector<std::vector<double>> vector(lines);
    for (int q = 0; q < lines; q++)
        vector[q] = std::vector<double>(columns);
    this->setMatrix(vector);
}


Matrix Matrix::transpose() {
    Matrix new_matrix(_size.second, _size.first);
    for (int q = 0; q < _size.second; q++)
        for (int w = 0; w < _size.first; w++)
            new_matrix[q][w] = matrix[w][q];
    return new_matrix;
} //

double Matrix::determinant() {
    std::vector<std::vector<double>> matrix = this->matrix;
    for (int q = 0; q < matrix.size(); q++) {
        if (matrix[q][q] == 0) {
            int not_null = q;
            while (matrix[not_null][q] == 0 and not_null < matrix.size());
            not_null++;
            matrix[q] = sum_line(matrix[q], matrix[not_null]);
        }
        for (int w = q + 1; w < matrix.size(); w++)
            matrix[w] = subtract_line(matrix[w],multiply_line(matrix[q],matrix[w][q] / matrix[q][q]));
    }
    double det = 1;
    for (int q = 0; q < matrix.size(); q++)
        det *= matrix[q][q];
    return det;
} //

Matrix Matrix::inverse() {
    Matrix cofactor(_size.first, _size.second);
    for (int q = 0; q < _size.first; q++)
        for (int w = 0; w < _size.second; w++)
            cofactor[q][w] = get_minor(q, w).determinant() * pow(-1, q + w);
    return cofactor.transpose() * (1 / determinant());
} //

Matrix operator*(Matrix matrix, const double number) {
    for (int q = 0; q < matrix.size().first; q++)
        for (int w = 0; w < matrix.size().second; w++)
            matrix[q][w] *= number;
    return matrix;
} //

Matrix operator*(const double number, Matrix matrix) {
    return operator*(matrix, number);
} //


std::vector<double> Matrix::subtract_line(std::vector<double> vector_0, std::vector<double> vector_1) {
    for (int q = 0; q < vector_0.size(); q++)
        vector_0[q] -= vector_1[q];
    return vector_0;
} //

std::vector<double> Matrix::sum_line(std::vector<double> vector_0, std::vector<double> vector_1) {
    for (int q = 0; q < vector_0.size(); q++)
        vector_0[q] += vector_1[q];
    return vector_0;
} //

std::vector<double> Matrix::multiply_line(std::vector<double> vector, double number) {
    for (int q = 0; q < vector.size(); q++)
        vector[q] *= number;
    return vector;
} //

Matrix Matrix::get_minor(int a, int b) {
    std::vector<std::vector<double>> minor;
    for (int q = 0; q < _size.first; q++) {
        std::vector<double> minor_line;
        if (q == a) continue;
        for (int w = 0; w < _size.second; w++) {
            if (w == b) continue;
            minor_line.push_back(matrix[q][w]);
        }
        minor.push_back(minor_line);
    }
    return Matrix(minor);
} //


String Matrix::toString(std::string str) {
    String raw;
    for (std::vector<double> line: matrix) {
        for (int q = 0; q < line.size(); q++) {
            if (q != 0)
                raw += str;
            raw += std::to_string(line[q]);
        }
        raw += "\n";
    }
    return raw;
} //

void Matrix::save(std::string file_path, std::string str) {
    std::ofstream file(file_path, std::ios_base::app);
    if (!file.is_open())
        throw std::runtime_error("path error");
    file << toString(str);
    file.close();
} //


std::pair<int, int> Matrix::size() {
    return _size;
}

std::vector<double> &Matrix::operator[](int number) {
    return matrix[number];
}

void Matrix::setMatrix(const std::vector<std::vector<double>> &matrix) {
    _size.first = matrix.size();
    _size.second = matrix[0].size();
    Matrix::matrix = matrix;
}
