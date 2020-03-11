//
// Created by marker on 02.03.2020.
//
#include "String.h"

std::vector<String> String::split(std::string str) {
    std::vector<String> strings;
    if (this->find(str) != -1) {
        String string(*this);
        String buffer;
        while (string.find(str) != -1) {
            int place = string.find(str);
            for (int q = 0; q < place; q++)
                buffer.push_back(string.at(q));
            strings.push_back(buffer);
            buffer.clear();
            string.erase(0, place + str.size());
        }
        strings.push_back(string);
    } else
        strings.push_back(*this);
    return strings;
};

String String::strip() {
        if (this->empty())
            return "";
        char first = this->at(0);
        char last = this->at(this->size() - 1);
        while (first == ' ' or first == '\t' or first == '\n' or first == '\0' or first == '\r' or first == '\377'){
            this->erase(0, 1);
            if (this->size() == 0)
                break;
            first = this->at(0);
        }
        if (this->size() != 0)
            while (last == ' ' or last == '\t' or last == '\n' or last == '\0' or last == '\r' or last == '\377') {
                this->erase(this->size() - 1);
                if (this->size() == 0)
                    break;
                last = this->at(this->size() - 1);
            }
        return *this;


}

String String::format(int number) {
    String string = *this;
    int position = string.find("{}");
    if (position != -1) {
        string.erase(position, 2);
        string.insert(position, std::to_string(number));
        return string;
    } else
        return string;
}

String String::format(unsigned int number) {
    String string = *this;
    int position = string.find("{}");
    if (position != -1) {
        string.erase(position, 2);
        string.insert(position, std::to_string(number));
        return string;
    } else
        return string;
}

String String::format(String string_) {
    String string = *this;
    int position = string.find("{}");
    if (position != -1) {
        string.erase(position, 2);
        string.insert(position, string_);
        return string;
    } else
        return string;
}

String String::format(double number) {
    String string = *this;
    int position = string.find("{}");
    if (position != -1) {
        string.erase(position, 2);
        string.insert(position, std::to_string(number));
        return string;
    } else
        return string;
}