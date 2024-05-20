cmake_minimum_required(VERSION 3.21)
project(crcwrk_intprtr)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(lexer)
add_subdirectory(parser)
add_subdirectory(tests)

configure_file(assets/stream.txt ${CMAKE_CURRENT_BINARY_DIR}/stream.txt COPYONLY)