#include "Matrix.h"
#include <iostream>

/* Матрица из TXT файла file_path под номером number */
Matrix::Matrix(std::string file_path, int number = 0) {
    std::ifstream file(file_path);
    if (!file.is_open())
        throw std::runtime_error("Path error!");

    /* считывание всего файла в строку */
    String string;
    while (!file.eof())
        string += file.get();

    /* разделение на отдлеьные матрицы (через пустую строку) */
    std::vector<String> variants = string.split_to_vector("\n\n");
    if (number > variants.size())
        throw std::runtime_error("Matrix number error!");

    /* разделение на строки с отдельными числами (через 4 пробела)*/
    std::vector<std::vector<double>> matrix;
    std::vector<String> lines = variants[number].split_to_vector("\n");
    for (String line: lines) {
        std::vector<double> numbers;
        std::vector<String> numbers_string = line.split_to_vector("    ");
        for (String number: numbers_string)
            numbers.push_back(std::strtod(number.data(), NULL));
        matrix.push_back(numbers);
    }
    this->setMatrix(matrix);
}
/* Матрица из двумерного вектора */
Matrix::Matrix(std::vector<std::vector<double>> vector) {
    this->setMatrix(vector);
}
/* Матрица заданного размера (lines и columns) заполненная значением value */
Matrix::Matrix(int lines, int columns, double value) {
    std::vector<std::vector<double>> vector;
    for (int q = 0; q < lines; q++) {
        std::vector<double> line;
        for (int w = 0; w < columns; w++)
            line.push_back(value);
        vector.push_back(line);
    }
    this->setMatrix(vector);
}

/* Транспонирует объект */
void Matrix::transpose() {
    this->setMatrix(this->getTransposed().getMatrix());
}
/* Возвраащет транспонированную матрицу, не меняя оригинал */
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
double determinant_func(std::vector<std::vector<double>> matrix) {
    double determinant = 0;
    int sign = 0;
    if (matrix.size() == 3)
        return matrix[0][0] * matrix[1][1] * matrix[2][2] + matrix[0][1] * matrix[1][2] * matrix[2][0] + matrix[0][2] * matrix[1][0] * matrix[2][1] - matrix[0][2] * matrix[1][1] * matrix[2][0] - matrix[0][1] * matrix[1][0] * matrix[2][2] - matrix[0][0] * matrix[1][2] * matrix[2][1];
    if (matrix.size() == 2)
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    for (int q = 0; q < matrix.size(); q++) {
        std::vector<std::vector<double>> new_matrix;
        for (int w = 0; w < matrix.size(); w++) {
            std::vector<double> line;
            for (int e = 1; e < matrix.size(); e++)
                line.push_back(matrix[w][e]);
            if (w != q)
                new_matrix.push_back(line);
        }
        if (sign++ % 2 == 0)
            determinant += determinant_func(new_matrix) * matrix[q][0];
        else
            determinant -= determinant_func(new_matrix) * matrix[q][0];
    }
    return determinant;
}

double determinant_func_2(std::vector<std::vector<double>> matrix){
    
}

double Matrix::determinant(){
    return determinant_func(this->matrix);
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
    for (std::vector<double> line: this->getMatrix()) {
        for (double number: line)
            raw += str + std::to_string(number);
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
    file << this->toRaw();
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
