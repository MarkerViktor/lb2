//
// Created by marker on 25.02.2020.
//
#ifndef LB2_MATRIX_H
#define LB2_MATRIX_H

#include <vector>
#include <fstream>
#include "String.h"

class Matrix {

private:
    std::pair<int, int> _size; // first – количество строк, second – количество столбцов
    std::vector<std::vector<double>> matrix;  // Матрица

    Matrix(std::vector<std::vector<double>> vector);
    Matrix(int lines, int columns);
public:
    Matrix() = default;

    Matrix(std::string file_path, int number = 0, std::string str = "    ");

    Matrix transpose(); // Возвраащет транспонированную матрицу, не меняя оригинал

    double determinant();

    Matrix inverse();

    friend Matrix operator*(Matrix matrix, double number);

    friend Matrix operator*(double number, Matrix matrix);

    String toString(std::string str = "    "); // Возвращает строку с матрицей построчно

    void save(std::string file_path, std::string str = "    ");

    std::pair<int, int> size();

    std::vector<double> &operator[](int number);

private:
    void setMatrix(const std::vector<std::vector<double>> &matrix);

    Matrix get_minor(int a, int b);

    static std::vector<double> subtract_line(std::vector<double> vector_0, std::vector<double> vector_1);

    static std::vector<double> sum_line(std::vector<double> vector_0, std::vector<double> vector_1);

    static std::vector<double> multiply_line(std::vector<double> vector, double number);
};

#endif //LB2_MATRIX_H
