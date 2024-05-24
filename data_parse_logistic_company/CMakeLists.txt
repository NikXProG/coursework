cmake_minimum_required(VERSION 3.21)
project(crcwrk_dt_prs_logic_company)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(tests)

add_library(
        crcwrk_dt_prs_logic_company
        src/employee.cpp)

target_include_directories(
        crcwrk_dt_prs_logic_company
        PUBLIC
        ./include)

set_target_properties(
    crcwrk_dt_prs_logic_company PROPERTIES
    LANGUAGES CXX
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    VERSION 1.0
    DESCRIPTION "parser implementation library")

configure_file(assets/file.txt ${CMAKE_CURRENT_BINARY_DIR}/file.txt COPYONLY)