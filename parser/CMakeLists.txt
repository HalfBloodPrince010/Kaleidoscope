add_library(parser
     parser.cpp
     parser.h
)

target_include_directories(parser PRIVATE ${CMAKE_SOURCE_DIR}/ast)
target_include_directories(parser PRIVATE ${CMAKE_SOURCE_DIR}/lexer)
target_include_directories(parser PRIVATE ${CMAKE_SOURCE_DIR}/logger)

target_link_libraries(parser PRIVATE ast)
target_link_libraries(parser PRIVATE lexer)
target_link_libraries(parser PRIVATE logger)