cmake_minimum_required(VERSION 3.8.0)
project(crcwrk_intprtr_tests 
        VERSION 1.0
        DESCRIPTION "logger libraries"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# Создание исполняемого файла crcwrk_prsr_tests
add_executable(crcwrk_intprtr_tests lexer_tests.cpp)

# Указание зависимостей для crcwrk_prsr_tests
target_link_libraries(
    crcwrk_intprtr_tests 
    PUBLIC
     crcwrk_intprtr_lxr)
target_link_libraries(
    crcwrk_intprtr_tests
    PUBLIC
    crcwrk_intprtr_prsr)

set_target_properties(
    crcwrk_intprtr_tests PROPERTIES
    LANGUAGES CXX
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    VERSION 1.0
    DESCRIPTION "parser implementation library tests"
)