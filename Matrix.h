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
public:
    Matrix() {};

    Matrix(std::string file_path, int number = 0, std::string str = "    ");

    Matrix(std::vector<std::vector<double>> vector);

    Matrix transpose(); // Возвраащет транспонированную матрицу, не меняя оригинал

    double determinant();

    Matrix inverse();

    friend Matrix operator*(Matrix matrix, double number);

    friend Matrix operator*(double number, Matrix matrix);

    String toRaw(std::string str = "    "); // Возвращает строку с матрицей построчно

    void save(std::string file_path, std::string str = "    ");

    std::pair<int, int> size();

    std::vector<double> &operator[](int number);

    void setMatrix(const std::vector<std::vector<double>> &matrix);

private:
    std::vector<std::vector<double>> get_minor(int a, int b);

    std::vector<std::vector<double>> from_file(std::string file_path, int number, std::string str);

    std::vector<double> subtract_line(std::vector<double> vector_0, std::vector<double> vector_1);

    std::vector<double> sum_line(std::vector<double> vector_0, std::vector<double> vector_1);

    std::vector<double> multiply_line(std::vector<double> vector, double number);

    double get_determinant(std::vector<std::vector<double>> matrix);
};

#endif //LB2_MATRIX_H
