#include "Matrix.h"
#include <iostream>
#include <cmath>

/* Матрица из TXT файла file_path под номером number, разделенная строкой str*/
Matrix::Matrix(std::string file_path, int number, std::string str) {
    std::ifstream file(file_path);
    if (!file.is_open())
        throw std::runtime_error("Path error!");

    /* считывание всего файла в строку */
    String string;
    while (!file.eof())
        string += file.get();

    /* разделение на отдельные матрицы (через пустую строку) */
    std::vector<String> variants = string.strip().split_to_vector("\n\n");
    if (number > variants.size()-1)
        throw std::runtime_error("Matrix number error!");

    /* разделение на строки с отдельными числами (через 4 пробела)*/
    std::vector<std::vector<double>> matrix;
    std::vector<String> lines = variants[number].strip().split_to_vector("\n");
    if (lines.size() < 2)
        throw std::runtime_error("Matrix error!");

    for (String line: lines) {
        std::vector<double> numbers;
        std::vector<String> numbers_string = line.strip().split_to_vector(str);
        for (String number: numbers_string)
            numbers.push_back(std::strtod(number.data(), NULL));
        matrix.push_back(numbers);
    }
    this->setMatrix(matrix);
}
Matrix::Matrix(std::vector<std::vector<double>> vector) {
    this->setMatrix(vector);
}
Matrix::Matrix(int lines, int columns) {
    std::vector<std::vector<double>> vector(lines);
    for (int q = 0; q < lines; q++) {
        std::vector<double> line(columns);
        vector[q] = line;
    }
    this->setMatrix(vector);
}

/* Транспонирует объект */
void Matrix::transpose() {
    this->setMatrix(this->getTransposed().getMatrix());
}\
Matrix Matrix::getTransposed() {
    std::vector<std::vector<double>> new_matrix;
    for (int q = 0; q < this->_size.second; q++) {
        std::vector<double> line;
        for (int w = 0; w < this->_size.first; w++)
            line.push_back(matrix[w][q]);
        new_matrix.push_back(line);
    }
    return Matrix(new_matrix);
}
Matrix Matrix::operator!() {
    return this->getTransposed();
}

/* Определитель */
std::vector<double> subtract_line(std::vector<double> vector_0, std::vector<double> vector_1) {
    for (int q = 0; q < vector_0.size(); q++)
        vector_0[q] -= vector_1[q];
    return vector_0;
}
std::vector<double> sum_line(std::vector<double> vector_0, std::vector<double> vector_1) {
    for (int q = 0; q < vector_0.size(); q++)
        vector_0[q] += vector_1[q];
    return vector_0;
}
std::vector<double> multiply_line(std::vector<double> vector, double number) {
    for (int q = 0; q < vector.size(); q++)
        vector[q] *= number;
    return vector;
}
double get_determinant(std::vector<std::vector<double>> matrix) {
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

/* Обратная матрица */
std::vector<std::vector<double>> Matrix::get_minor(int a, int b) {
    std::vector<std::vector<double>> minor;
    for (int q = 0; q < matrix.size(); q++) {
        std::vector<double> new_line;
        if (a == q)
            continue;
        for (int w = 0; w < matrix[q].size(); w++) {
            if (w == b)
                continue;
            new_line.push_back(matrix[q][w]);
        }
        minor.push_back(new_line);
    }
    return minor;
}
void Matrix::inverse(){
    setMatrix(getInverse().getMatrix());
}
Matrix Matrix::getInverse() {
    double determinant_ = get_determinant(matrix);
    Matrix complement(_size.first, _size.second);
    for (int q = 0; q < matrix.size(); q++)
        for (int w = 0; w < matrix.size(); w++)
            complement[q][w] = get_determinant(get_minor(q, w)) * pow(-1, q+w);
    return complement.getTransposed() * (1/determinant_);
}

/* Операторы умножения */
Matrix operator*(Matrix matrix, const double number){
    for (int q = 0; q < matrix.size().first; q++)
        for (int w = 0; w < matrix.size().second; w++)
            matrix[q][w] *= number;
    return matrix;
}
Matrix operator*(const double number, Matrix matrix){
    return operator*(matrix, number);
}

/* Возвращает строку, содержащую матрицу построчно */
std::string Matrix::toRaw(std::string str) {
    std::string raw;
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
/* Сохраняет матрицу в конец файл file_path */
void Matrix::save(std::string file_path, std::string str) {
    std::ofstream file(file_path, std::ios_base::app);
    if (!file.is_open())
        throw std::runtime_error("Path error!");
    file << this->toRaw(str);
}


/* Возваращет пару из кол-ва строк и столбцов */
std::pair<int, int> Matrix::size() {
    return _size;
}

std::vector<double> &Matrix::operator[](int number) {
    return matrix[number];
}

std::vector<std::vector<double>> &Matrix::getMatrix() {
    return matrix;
}

void Matrix::setMatrix(const std::vector<std::vector<double>> &matrix) {
    _size.first = matrix.size();
    _size.second = matrix[0].size();
    Matrix::matrix = matrix;
}
