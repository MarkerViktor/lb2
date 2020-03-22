#include <iostream>
#include <map>
#include "Matrix.h"
#include "String.h"

using namespace std;
map<String, Matrix> *matrices;

void add_matrix(String name, Matrix matrix) {
    if (matrices->find(name) != matrices->end())
        throw runtime_error("already exist error");
    matrices->operator[](name) = matrix;
}

Matrix& get_matrix(String name) {
    if (matrices->find(name) != matrices->end())
        return matrices->at(name);
    else
        throw runtime_error("unknown matrix error");
}


String get_separator(String separator_number_string) {
    if (separator_number_string == "1")
        return  "\t";
    else if (separator_number_string == "2")
        return  "    ";
    else if (separator_number_string == "3")
        return  "  ";
    else
        throw runtime_error("separator error");
}

pair<String, String> get_file_args(String parameters_string) {
    vector<String> parameters = parameters_string.split("|");
    String separator, path = parameters[0];
    string extension = path.substr(path.size() - 4);
    if (extension == ".txt")
        separator = "    ";
    else if (extension == ".csv")
        separator = ";";
    else
        throw runtime_error("path error");
    return pair<String, String>(path, separator);
}


String help() {
    return "\r<--------------------------------------------HELP------------------------------------------->\n\n"
           " - PRINT {matrix name}|{separator \"1\" - tabulation, \"2\" - 4 spaces, \"3\" - 2 spaces}\n"
           " - LOAD {new matrix name} FROM {path}|{separator number ^ } NUMBER {matrix number}\n"
           " - SAVE {matrix name} TO {path}|{separator number ^ }\n"
           " - TRANSPOSE {matrix name} TO {new matrix name}\n"
           " - INVERSE {matrix name} TO {new matrix name}\n"
           " - MULTIPLY {matrix name} BY {number} TO {new matrix name}\n"
           " - DETERMINANT {matrix name}\n"
           " - CLEAN\n"
           " - EXIT\n";
}

String print(vector<String> command_parts) { // PRINT matrix|1
    if (command_parts.size() != 2)
        throw runtime_error("parameters error");

    String parameters_string = command_parts[1];
    String matrix_name;
    String separator;

    if (parameters_string.find("|") != -1) {
        vector<String> parameters = parameters_string.split("|");
        if (parameters.size() != 2)
            throw runtime_error("parameters error");
        matrix_name = parameters[0];
        separator = get_separator(parameters[1]);
    } else {
        matrix_name = parameters_string;
        separator = "  ";
    }

    Matrix &matrix = get_matrix(matrix_name);
    String raw = matrix.toString(separator);
    int rows = matrix.size().first;
    int columns = matrix.size().second;
    return String("Matrix \"{}\" [{}, {}]\n\n{}").format(matrix_name).format(rows).format(columns).format(raw);
}

String load(vector<String> command_parts) { // LOAD matrix FROM path|1 NUMBER 6
    if (command_parts[2] != "FROM" or (command_parts.size() != 4 and (command_parts.size() != 6 or command_parts[4] != "NUMBER")))
        throw runtime_error("parameters error");

    String matrix_name = command_parts[1];

    String file_string = command_parts[3];
    String path, separator;
    if (file_string.find("|") != -1) {
        vector<String> parameters = file_string.split("|");
        if (parameters.size() != 2)
            throw runtime_error("parameters error");
        path = parameters[0];
        separator = get_separator(parameters[1]);
    } else {
        pair<String, String> parameters = get_file_args(file_string);
        path = parameters.first;
        separator = parameters.second;
    }

    int number;
    if (command_parts.size() == 6)
        number = strtol(command_parts[5].data(), NULL, 10);
    else
        number = 0;

    Matrix matrix(path, number, separator);
    add_matrix(matrix_name, matrix);
    return String("New matrix \"{}\" was loaded").format(matrix_name);
}

String save(vector<String> command_parts) { // SAVE matrix TO path|1
    if (command_parts[2] != "TO" or command_parts.size() != 4)
        throw runtime_error("parameters error");

    String matrix_name = command_parts[1];
    String file_string = command_parts[3];
    String path, separator;
    if (file_string.find("|") != -1) {
        vector<String> parameters = file_string.split("|");
        if (parameters.size() != 2)
            throw runtime_error("parameters error");
        path = parameters[0];
        separator = get_separator(parameters[1]);
    } else {
        pair<String, String> parameters = get_file_args(file_string);
        path = parameters.first;
        separator = parameters.second;
    }

    Matrix &matrix = get_matrix(matrix_name);

    matrix.save(path, separator);
    return String("Matrix \"{}\" was saved to \"{}\"").format(matrix_name).format(path);
}

String transpose(vector<String> command_parts) { // TRANSPOSE matrix_a TO matrix_b
    if (command_parts.size() != 2 and (command_parts[2] != "TO" or command_parts.size() != 4))
        throw runtime_error("parameters error");
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

String inverse(vector<String> command_parts) { // INVERSE matrix_a TO matrix_b
    if (command_parts.size() != 2 and (command_parts[2] != "TO" or command_parts.size() != 4))
        throw runtime_error("parameters error");
    String matrix_name = command_parts[1];
    Matrix &matrix = get_matrix(matrix_name);

    if (command_parts.size() == 2) {
        matrix = matrix.inverse();
        return String("Matrix \"{}\" was inverted").format(matrix_name);
    } else if(command_parts.size() == 4) {
        String matrix_to_name = command_parts[3];
        add_matrix(matrix_to_name, matrix.inverse());
        return String("New matrix \"{}\" is inverted matrix \"{}\"").format(matrix_to_name).format(matrix_name);
    }
}

String determinant(vector<String> command_parts) { // DETERMINANT matrix
    if (command_parts.size() != 2)
        throw runtime_error("parameters error");
    String matrix_name = command_parts[1];
    Matrix &matrix = get_matrix(matrix_name);

    return String("A determinant of matrix \"{}\" = {}").format(matrix_name).format(matrix.determinant());
}

String multiply(vector<String> command_parts) { //MULTIPLY matrix_a BY number TO matrix_b
    if (!(command_parts[2] == "BY" and (command_parts.size() == 4 or (command_parts[4] == "TO" and command_parts.size() == 6))))
        throw runtime_error("parameters error");
    String matrix_name = command_parts[1];
    Matrix &matrix = get_matrix(matrix_name);
    double number = strtod(command_parts[3].data(), NULL);

    if (command_parts.size() == 4) {
        matrix = matrix * number;
        return String("Matrix \"{}\" was multiplied by {}").format(matrix_name).format(number);
    } else  {
        String matrix_to_name = command_parts[5];
        add_matrix(matrix_to_name, matrix * number);
        return String("New matrix \"{}\" is multiplied by {} matrix \"{}\"").format(matrix_to_name).format(number).format(matrix_name);
    }
}

int main() {
    matrices = new map<String, Matrix>;
    String input;
    String output;
    system("color 70");
    cout << "* To see HELP enter empty command\n";

    while (true) {
        map<String, Matrix> m = *matrices;
        cout << ">>>";

        getline(cin, input);
        cout << "\n  ";

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

            else if (command == "MULTIPLY")
                output = multiply(parts);

            else if (command == "CLEAN")
                    system("CLS");

            else if (command == "EXIT")
                    break;
            else
                output = "! Unknown command";

        } catch (runtime_error exception) {
            String what = exception.what();
            if (what == "unknown matrix error")
                output = "! Unknown matrix name";

            else if (what == "already exist error")
                output = "! Matrix with this name already exist";

            else if (what == "path error")
                output = "! File's path error";

            else if (what == "matrix number error")
                output = "! Matrix with the number is not found";

            else if (what == "parameters error")
                output = "! Function's parameters error";

            else
                output = "! Unknown error";
        }

        cout << output << "\n\n";
        output.clear();
    }
}

