cmake_minimum_required(VERSION 3.8.0)
project(crcwrk_mmorpg_sys_skll_tests
        VERSION 1.0
        DESCRIPTION "logger libraries"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# Создание исполняемого файла crcwrk_prsr_tests
add_executable(crcwrk_mmorpg_sys_skll_tests tests.cpp)

target_link_libraries(
    crcwrk_mmorpg_sys_skll_tests
        PUBLIC
        crcwrk_mmorpg_sys_skll)
set_target_properties(
    crcwrk_mmorpg_sys_skll_tests PROPERTIES
    LANGUAGES CXX
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    VERSION 1.0
    DESCRIPTION "parser implementation library tests"
)