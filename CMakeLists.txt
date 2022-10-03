cmake_minimum_required(VERSION 3.10)
project(ExpressionCalc)

set(CMAKE_CXX_STANDARD 11)

# Address Sanitizer
if (NOT DISABLE_ASAN)
    set(ASAN_FLAGS "-fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer")
    add_definitions(${ASAN_FLAGS})
    set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} ${ASAN_FLAGS})
endif ()

include_directories(src)
add_subdirectory(src)

file(GLOB APP_SOURCES
        example/xCalc.cpp
        )

# Executable
add_executable(xCalc ${APP_SOURCES})

# Link
target_link_libraries(xCalc expression_calc)
