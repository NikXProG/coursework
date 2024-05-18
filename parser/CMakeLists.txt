cmake_minimum_required(VERSION 3.21)
project(crcwrk_prsr)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(tests)

add_library(
        crcwrk_prsr
        src/parser_builder.cpp)
target_include_directories(
        crcwrk_prsr
        PUBLIC
        ./include)
set_target_properties(
    crcwrk_prsr PROPERTIES
    LANGUAGES CXX
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    VERSION 1.0
    DESCRIPTION "parser implementation library")

configure_file(assets/stream.txt ${CMAKE_CURRENT_BINARY_DIR}/stream.txt COPYONLY)