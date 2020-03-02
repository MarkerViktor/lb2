#include "Matrix.h"
#include <iostream>

        /* Конструкторы */
// Матрица из TXT файла file_path под номером number
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
    std::vector<String> lines = variants[number - 1].split_to_vector("\n");
    for (String line: lines) {
        std::vector<double> numbers;
        std::vector<String> numbers_string = line.split_to_vector("    ");
        for (String number: numbers_string)
            numbers.push_back(std::strtod(number.data(), NULL));
        matrix.push_back(numbers);
    }
    this->setMatrix(matrix);
}

// матрица из двумерного вектора
Matrix::Matrix(std::vector<std::vector<double>> vector) {
    this->setMatrix(vector);
}

// матрица заданного размера (lines и columns) заполненная значением value
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

        /* Транспонирование */
// транспонирует вызвавший объект
void Matrix::transpose() {
    this->setMatrix(this->getTransposed().getMatrix());
}

// возвращает объект транспонированной матрицы (вызвавший объект не изменяется)
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

        /* Ввод|Вывод */
// Возвращает строку, содержащую матрицу построчно
String Matrix::getRaw(std::string str = "  ") {
    String raw;
    for (std::vector<double> line: this->getMatrix()) {
        for (double number: line)
            raw += std::to_string(number) + str;
        raw += "\n";
    }
    return raw;
}

// Возваращет пару из кол-ва строк и столбцов
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
