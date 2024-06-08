cmake_minimum_required(VERSION 3.21)
project(crcwrk_intprtr_lxr)

set(CMAKE_CXX_STANDARD 17)

add_library(
        crcwrk_intprtr_lxr
        src/lexer.cpp
        src/token.cpp
        src/lexer_builder.cpp)
target_include_directories(
        crcwrk_intprtr_lxr
        PUBLIC
        ./include)
set_target_properties(
    crcwrk_intprtr_lxr PROPERTIES
    LANGUAGES CXX
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    VERSION 1.0
    DESCRIPTION "parser implementation library")