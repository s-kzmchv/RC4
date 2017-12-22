cmake_minimum_required(VERSION 3.9)
project(RC4)

set(CMAKE_CXX_STANDARD 11)

add_executable(RC4 main.cpp RC4algorithm.cpp RC4algorithm.h ProcessingData.cpp ProcessingData.h)