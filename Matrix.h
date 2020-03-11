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
    Matrix(std::string file_path, int number = 0, std::string str = "    ");
    Matrix(std::vector<std::vector<double>> vector);
    Matrix(int lines, int columns);
    Matrix();

    void fromFile(std::string file_path, int number, std::string str);

    Matrix transpose(); // Возвраащет транспонированную матрицу, не меняя оригинал
    double determinant();
    Matrix inverse();

    friend Matrix operator*(Matrix matrix, double number);
    friend Matrix operator*(double number, Matrix matrix);

private:
    std::vector<std::vector<double>> get_minor(int a, int b);

public:
    String toRaw(std::string str = "    "); // Возвращает строку с матрицей построчно

    void save(std::string file_path, std::string str = "    ");

    std::pair<int, int> size();

    std::vector<double> &operator[](int number);

    std::vector<std::vector<double>> &getMatrix();

    void setMatrix(const std::vector<std::vector<double>> &matrix);
};

#endif //LB2_MATRIX_H
