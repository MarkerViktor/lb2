//
// Created by marker on 02.03.2020.
//

#ifndef LB2_STRING_H
#define LB2_STRING_H

#include <string>
#include <vector>

class String : public std::string {
public:
    std::vector<String> split(std::string str);

    String strip();

    String format(int number);
    String format(unsigned int number);
    String format(String string);
    String format(const std::string string);
    String format(double number);

    String(const char *string) : std::string(string) {};

    String() : std::string("") {};
};
#endif //LB2_STRING_H
