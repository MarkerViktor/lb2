#include <iostream>
#include "Matrix.h"
#include "String.h"

using namespace std;

int main() {
    setlocale(LC_ALL, "ru");
    Matrix matrix("/Users/mihailnefedov/CLionProjects/lb2/Task.txt", 6);
    cout << matrix.determinate() << endl;
}

