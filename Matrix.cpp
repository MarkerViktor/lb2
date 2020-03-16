#include "Matrix.h"
#include <iostream>
#include <cmath>

/* �������� ������� �� ����� */
std::vector<std::vector<double>> Matrix::from_file(std::string file_path, int number, std::string str) {
    std::ifstream file(file_path);
    if (!file.is_open())
        throw std::runtime_error("path error");

    /* ���������� ����� ����� � ������ */
    String string;
    while (!file.eof())
        string += file.get();

    /* ���������� �� ��������� ������� (����� ������ ������) */
    std::vector<String> variants = string.strip().split("\n\n");
    if (number > variants.size() - 1)
        throw std::runtime_error("matrix number error");

    /* ���������� �� ������ � ���������� ������� (����� 4 �������)*/
    std::vector<std::vector<double>> matrix;
    std::vector<String> lines = variants[number].strip().split("\n");
    if (lines.size() < 2)
        throw std::runtime_error("matrix error");

    for (String line: lines) {
        std::vector<double> numbers;
        std::vector<String> numbers_string = line.strip().split(str);
        for (String number: numbers_string) {
            if (number.size() == 0) // �������� �� ������ ����� ��� ���������� �����������
                continue;
            numbers.push_back(std::strtod(number.data(), NULL));
        }
        if (numbers.size() == 0) // �������� �� ������ ������ ��� ���������� ����������� \n
            continue;
        matrix.push_back(numbers);
    }
    return matrix;
}

/* ������� �� TXT ����� file_path ��� ������� number, ����������� ������� str*/
Matrix::Matrix(std::string file_path, int number, std::string str) {
    std::vector<std::vector<double>> vector = from_file(file_path, number, str);
    this->setMatrix(vector);
}

Matrix::Matrix(std::vector<std::vector<double>> vector) {
    this->setMatrix(vector);
}

/* ���������� ����������������� ������� */
Matrix Matrix::transpose() {
    std::vector<std::vector<double>> new_matrix(this->_size.second);
    for (int q = 0; q < this->_size.second; q++) {
        std::vector<double> line(this->_size.first);
        for (int w = 0; w < this->_size.first; w++)
            line[w] = matrix[w][q];
        new_matrix[q] = line;
    }
    return Matrix(new_matrix);
}

/* ������������ */
std::vector<double> Matrix::subtract_line(std::vector<double> vector_0, std::vector<double> vector_1) {
    for (int q = 0; q < vector_0.size(); q++)
        vector_0[q] -= vector_1[q];
    return vector_0;
}

std::vector<double> Matrix::sum_line(std::vector<double> vector_0, std::vector<double> vector_1) {
    for (int q = 0; q < vector_0.size(); q++)
        vector_0[q] += vector_1[q];
    return vector_0;
}

std::vector<double> Matrix::multiply_line(std::vector<double> vector, double number) {
    for (int q = 0; q < vector.size(); q++)
        vector[q] *= number;
    return vector;
}

double Matrix::get_determinant(std::vector<std::vector<double>> matrix) {
    for (int q = 0; q < matrix.size(); q++) {
        if (matrix[q][q] == 0) {
            int not_null = q;
            while (matrix[not_null][q] == 0)
                not_null++;
            matrix[q] = sum_line(matrix[q], matrix[not_null]);
        }
        for (int w = q + 1; w < matrix.size(); w++)
            matrix[w] = subtract_line(
                    matrix[w],
                    multiply_line(matrix[q], matrix[w][q] / matrix[q][q])
            );
    }
    double det = 1;
    for (int q = 0; q < matrix.size(); q++)
        det *= matrix[q][q];
    return det;
}

double Matrix::determinant() {
    return get_determinant(matrix);
}

/* �������� ������� */
std::vector<std::vector<double>> Matrix::get_minor(int a, int b) {
    std::vector<std::vector<double>> minor(this->_size.first-1);
    for (int q = 0; q < matrix.size(); q++) {
        std::vector<double> new_line(this->_size.second-1);
        if (a == q)
            continue;
        for (int w = 0; w < matrix[q].size(); w++) {
            if (w == b)
                continue;
            new_line[w] = matrix[q][w];
        }
        minor[q] = new_line;
    }
    return minor;
}

Matrix Matrix::inverse() {
    double determinant_ = get_determinant(matrix);
    std::vector<std::vector<double>> cofactor;
    for (int q = 0; q < matrix.size(); q++)
        for (int w = 0; w < matrix.size(); w++)
            cofactor[q][w] = get_determinant(get_minor(q, w)) * pow(-1, q + w);
    return Matrix(Matrix(cofactor).transpose() * (1 / determinant_));
}

/* ��������� ��������� */
Matrix operator*(Matrix matrix, const double number) {
    for (int q = 0; q < matrix.size().first; q++)
        for (int w = 0; w < matrix.size().second; w++)
            matrix[q][w] *= number;
    return matrix;
}

Matrix operator*(const double number, Matrix matrix) {
    return operator*(matrix, number);
}

/* ���������� ������, ���������� ������� ���������*/
String Matrix::toRaw(std::string str) {
    String raw;
    for (std::vector<double> line: matrix) {
        for (int q = 0; q < line.size(); q++) {
            if (q != 0)
                raw += str;
            raw += std::to_string(line[q]);
        }
        raw += "\n";
    }
    raw += "\n";
    return raw;
}

/* ��������� ������� � ����� ���� file_path */
void Matrix::save(std::string file_path, std::string str) {
    std::ofstream file(file_path, std::ios_base::app);
    if (!file.is_open())
        throw std::runtime_error("path error");
    file << this->toRaw(str);
    file.close();
}


/* ���������� ���� �� ���-�� ����� � �������� */
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
