cmake_minimum_required(VERSION 3.25)
project(markdown_to_pdf)

set(CMAKE_CXX_STANDARD 20)

add_executable(markdown_to_pdf src/main.cpp
        src/header/main.hpp)
