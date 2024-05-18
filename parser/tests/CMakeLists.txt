cmake_minimum_required(VERSION 3.8.0)
project(crcwrk_prsr_tests)

set(CMAKE_CXX_STANDARD 17)

# Создание исполняемого файла crcwrk_prsr_tests
add_executable(crcwrk_prsr_tests parser_tests.cpp)

# Указание зависимостей для crcwrk_prsr_tests
target_link_libraries(crcwrk_prsr_tests PUBLIC crcwrk_prsr)

set_target_properties(
    crcwrk_prsr_tests PROPERTIES
    LANGUAGES CXX
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    VERSION 1.0
    DESCRIPTION "parser implementation library tests"
)