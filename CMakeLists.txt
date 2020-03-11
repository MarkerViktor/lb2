cmake_minimum_required(VERSION 3.15)
project(lb2)

set(CMAKE_CXX_STANDARD 20)

add_executable(lb2 main.cpp Matrix.cpp Matrix.h String.cpp String.h)