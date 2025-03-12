cmake_minimum_required(VERSION 3.20)

project(kaleidoscope)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# LLVM
execute_process(COMMAND llvm-config --cxxflags
                OUTPUT_VARIABLE LLVM_CXXFLAGS
                OUTPUT_STRIP_TRAILING_WHITESPACE)

execute_process(COMMAND llvm-config --ldflags
                OUTPUT_VARIABLE LLVM_LDFLAGS
                OUTPUT_STRIP_TRAILING_WHITESPACE)

execute_process(COMMAND llvm-config --libs all
                OUTPUT_VARIABLE LLVM_LIBS
                OUTPUT_STRIP_TRAILING_WHITESPACE)
    
execute_process(COMMAND llvm-config --system-libs
                OUTPUT_VARIABLE LLVM_SYSTEM_LIBS
                OUTPUT_STRIP_TRAILING_WHITESPACE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LLVM_CXXFLAGS} -Wall -Werror")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LLVM_LDFLAGS}")
add_compile_options(-Wno-unused-private-field)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/ast
    ${CMAKE_SOURCE_DIR}/lexer
    ${CMAKE_SOURCE_DIR}/logger
    ${CMAKE_SOURCE_DIR}/parser
    ${CMAKE_SOURCE_DIR}/llvm_support
)

# Subdirectories
add_subdirectory(lexer)
add_subdirectory(ast)
add_subdirectory(logger)
add_subdirectory(parser)
add_subdirectory(llvm_support)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE lexer ast logger parser llvm_support ${LLVM_LIBS} ${LLVM_SYSTEM_LIBS})