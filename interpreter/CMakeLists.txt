cmake_minimum_required(VERSION 3.21)
project(crcwrk_intprtr)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(lexer)
add_subdirectory(tests)
add_subdirectory(parser)

configure_file(assets/stream.txt ${CMAKE_CURRENT_BINARY_DIR}/stream.txt COPYONLY)