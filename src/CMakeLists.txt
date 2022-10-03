cmake_minimum_required(VERSION 3.10)
project(ExpressionCalc)

set(CMAKE_CXX_STANDARD 11)

set(sources expression_calc.cpp)

add_library(expression_calc STATIC ${sources})
target_include_directories(expression_calc PUBLIC .)
