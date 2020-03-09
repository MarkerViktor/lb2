#include <iostream>
#include <array>
#include "Matrix.h"
#include "String.h"

using namespace std;
vector<Matrix> matrices;

int main() {
    setlocale(LC_ALL, "ru");
    cout << String("There are {} matrices. You can...\n").format(matrices.size());
}

