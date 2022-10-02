cmake_minimum_required(VERSION 3.23)
project(ExpressionCalc)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)
add_subdirectory(src)

file(GLOB APP_SOURCES
        example/xCalc.cpp
        )

# Executable
add_executable(xCalc ${APP_SOURCES})

# Link
target_link_libraries(xCalc expression_calc)
