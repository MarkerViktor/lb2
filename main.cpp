#include <iostream>
#include <windows.h>
#include <map>
#include "Matrix.h"
#include "String.h"

using namespace std;
map<String, Matrix> *matrices;

void add_matrix(String name, Matrix matrix) {
    if (matrices->find(name) != matrices->end())
        throw runtime_error("already exist");
    matrices->operator[](name) = matrix;
}

Matrix& get_matrix(String name) {
    if (matrices->find(name) != matrices->end())
        return matrices->at(name);
    else
        throw runtime_error("unknown matrix error");
}

String help() {
    return "<----------------------HELP--------------------->\n"
           " - print {matrix name}\n"
           " - load {new matrix name} from {path}|{matrix number}\n"
           " - save {matrix name} to {path}\n"
           " - transpose {matrix name} to {new matrix name}\n"
           " - inverse {matrix name} to {new matrix name}\n"
           " - determinate {matrix name}";
}

String print(vector<String> command_parts) {
    if (command_parts.size() != 2)
        return "! Command's \"PRINT\" parameters error";
    String matrix_name = command_parts[1];
    Matrix &matrix = get_matrix(matrix_name);

    String raw = matrix.toRaw("\t");
    int rows = matrix.size().first;
    int columns = matrix.size().second;
    return String("Matrix \"{}\" [{}, {}]\n\n{}").format(matrix_name).format(rows).format(columns).format(raw);
}

String load(vector<String> command_parts) {
    if (command_parts[2] != "FROM" or command_parts.size() != 4)
        return "! Command's \"LOAD\" parameters error";

    String matrix_name = command_parts[1];
    String parameters = command_parts[3];

    Matrix matrix;
    if (parameters.find("|") != -1) {
        vector<String> parameters = command_parts[3].split("|");

        String path = parameters[0];
        int matrix_number = strtol(parameters[1].data(), NULL, 10);
        matrix = Matrix(path, matrix_number);
    } else {
        String path = parameters;
        matrix = Matrix(path);
    }
    add_matrix(matrix_name, matrix);
    return String("New matrix \"{}\" was loaded").format(matrix_name);
}

String save(vector<String> command_parts) {
    if (command_parts[2] != "TO" or command_parts.size() != 4)
        return "! Command's \"SAVE\" parameters error";
    String matrix_name = command_parts[1];
    Matrix &matrix = get_matrix(matrix_name);

    String parameters = command_parts[3];
    matrix.save(parameters);
    return String("Matrix \"{}\" was saved to \"{}\"").format(matrix_name).format(parameters);
}

String transpose(vector<String> command_parts) {
    if (command_parts.size() != 2 and (command_parts[2] != "TO" or command_parts.size() != 4))
        return "! Command's \"TRANSPOSE\" parameters error";
    String matrix_name = command_parts[1];
    Matrix &matrix = get_matrix(matrix_name);

    if (command_parts.size() == 2) {
        matrix = matrix.transpose();
        return String("Matrix \"{}\" was transposed").format(matrix_name);
    } else if(command_parts.size() == 4) {
        String matrix_to_name = command_parts[3];
        add_matrix(matrix_to_name, matrix.transpose());
        return String("New matrix \"{}\" was assigned by transposed matrix \"{}\"").format(matrix_to_name).format(matrix_name);
    }
    throw runtime_error("error");
}

String inverse(vector<String> command_parts) {
    if (command_parts.size() != 2 and (command_parts[2] != "TO" or command_parts.size() != 4))
        return "! Command's \"INVERSE\" parameters error\n";
    String matrix_name = command_parts[1];
    Matrix &matrix = get_matrix(matrix_name);

    if (command_parts.size() == 2) {
        matrix = matrix.inverse();
        return String("Matrix \"{}\" was inverted\n").format(matrix_name);
    } else if(command_parts.size() == 4) {
        String matrix_to_name = command_parts[3];
        add_matrix(matrix_to_name, matrix.inverse());
        return String("New matrix \"{}\" was assigned by inverted matrix \"{}\"").format(matrix_to_name).format(matrix_name);
    }
    throw runtime_error("error");
}

String determinant(vector<String> command_parts) {
    if (command_parts.size() != 2)
        return "! Command's \"DETERMINATE\" parameters error";
    String matrix_name = command_parts[1];
    Matrix &matrix = get_matrix(matrix_name);

    return String("A determinant of matrix \"{}\" = {}").format(matrix_name).format(matrix.determinant());
}

int main() {
    matrices = new map<String, Matrix>;
    String input;
    String output;
    while (true) {
        map<String, Matrix> m = *matrices;
        cout << "\n\n>>>";

        getline(cin, input);
        cout << "\n";

        vector<String> parts = input.strip().split(" ");
        String command = parts[0];
        try {
            if (command.empty())
                output = help();
            /* Комнады */
            else if (command == "PRINT")
                output = print(parts);

            else if (command == "LOAD")
                output = load(parts);

            else if (command == "SAVE")
                output = save(parts);

            else if (command == "TRANSPOSE")
                output = transpose(parts);

            else if (command == "INVERSE")
                output = inverse(parts);

            else if (command == "DETERMINANT")
                output = determinant(parts);

            else if (parts[0] == "EXIT" and parts.size() == 1)
                break;
            else
                output = "! Unknown command\n";

        } catch (runtime_error exception) {
            String what = exception.what();
            if (what == "unknown matrix error")
                output = "! Unknown matrix name\n";

            else if (what == "already exist error")
                output = "! Matrix with this name already exist\n";

            else if (what == "path error")
                output = "! File path error";

            else if (what == "matrix number error")
                output = "! Matrix with the number is not found";

            else if (what == "error")
                output = "! Unknown error";
            else
                output = "! Unknown command\n";

        }

        cout << output;
    }
}

