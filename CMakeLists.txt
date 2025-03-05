cmake_minimum_required(VERSION 3.20)

project(kaleidoscope)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Subdirectories
add_subdirectory(lexer)
add_subdirectory(ast)
add_subdirectory(logger)
add_subdirectory(parser)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE lexer ast logger parser)