//
// Created by marker on 02.03.2020.
//
#include "String.h"

std::vector<String> String::split_to_vector(std::string str) {
    std::vector<String> strings;
    String *string = new String(this->c_str());
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
    char first = this->at(0);
    char last = this->at(this->size() - 1);
    while (first == ' ' or first == '\t' or first == '\n' or first == '\0' or first == '\r' or first == '\377'){
        this->erase(0, 1);
        first = this->at(0);
    }

    while (last == ' ' or last == '\t' or last == '\n' or last == '\0' or last == '\r' or last == '\377') {
        this->erase(this->size() - 1);
        last = this->at(this->size() - 1);
    }
    return *this;
}

String String::format(int number) {
    int position = this->find("{}");
    if (position != -1) {
        this->erase(position, 2);
        this->insert(position, std::to_string(number));
        return *this;
    } else
        return *this;
}

String String::format(unsigned int number) {
    int position = this->find("{}");
    if (position != -1) {
        this->erase(position, 2);
        this->insert(position, std::to_string(number));
        return *this;
    } else
        return *this;
}

String String::format(String string) {
    int position = this->find("{}");
    if (position != -1) {
        this->erase(position, 2);
        this->insert(position, string);
        return *this;
    } else
        return *this;
}

String String::format(const std::string string) {
    int position = this->find("{}");
    if (position != -1) {
        this->erase(position, 2);
        this->insert(position, string);
        return *this;
    } else
        return *this;
}

String String::format(double number) {
    int position = this->find("{}");
    if (position != -1) {
        this->erase(position, 2);
        this->insert(position, std::to_string(number));
        return *this;
    } else
        return *this;
}
