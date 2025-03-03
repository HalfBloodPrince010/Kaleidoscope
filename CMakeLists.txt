cmake_minimum_required(VERSION 3.20)

project(kaleidoscope)

set(CMAKE_CXX_STANDARD 20)

# Subdirectories
add_subdirectory(lexer)
add_subdirectory(ast)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE lexer)
target_link_libraries(${PROJECT_NAME} PRIVATE ast)