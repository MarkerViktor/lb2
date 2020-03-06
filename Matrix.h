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
    Matrix(std::string file_path, int number = 0);

    Matrix(std::vector<std::vector<double>> vector);

    Matrix(int lines, int columns, double value);

public:
    void transpose(); // Транспонирует объект
    Matrix getTransposed(); // Возвраащет транспонированную матрицу, не меняя оригинал
    Matrix operator!();

    double determinate();

    Matrix inverse();

    friend Matrix operator*(Matrix matrix, double number);

    friend Matrix operator*(double number, Matrix matrix);

public:
    std::string toRaw(std::string str = "    "); // Возвращает строку с матрицей построчно

    void save(std::string file_path, std::string str = "   ");

    std::pair<int, int> size();

    std::vector<double> &operator[](int number);

    std::vector<std::vector<double>> &getMatrix();

    void setMatrix(const std::vector<std::vector<double>> &matrix);
};

#endif //LB2_MATRIX_H
