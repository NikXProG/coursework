cmake_minimum_required(VERSION 3.21)
project(crcwrk_txt_trans)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

set(CMAKE_CXX_STANDARD 14)


add_executable(crcwrk_txt_trans main.cpp)
target_link_libraries(crcwrk_txt_trans PRIVATE nlohmann_json::nlohmann_json)
include_directories(${CMAKE_SOURCE_DIR}/assets)

set_target_properties(
    crcwrk_txt_trans PROPERTIES
    LANGUAGES CXX
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    VERSION 1.0
    DESCRIPTION "parser implementation library")