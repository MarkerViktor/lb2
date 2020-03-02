//
// Created by marker on 02.03.2020.
//
#include "String.h"

std::vector<String> String::split_to_vector(std::string str) {
    std::vector<String> strings;
    String *string = new String(this->c_str());
    string->strip();
    String buffer;
    while (string->find(str) != -1) {
        int place = string->find(str);
        for (int q = 0; q < place; q++)
            buffer.push_back(string->at(q));
        strings.push_back(buffer);
        buffer.clear();
        string->erase(0, place + str.size());
    }
    strings.push_back(*string);
    delete (string);
    return strings;
}

std::string *String::split(std::string str) {
    return split_to_vector(str).data();
};

String String::strip() {
    while (this->at(0) == ' ' || this->at(0) == '\t')
        this->erase(0, 1);
    while (this->at(this->size() - 1) == ' ' || this->at(0) == '\t')
        this->erase(this->size() - 1);
    return *this;
}
