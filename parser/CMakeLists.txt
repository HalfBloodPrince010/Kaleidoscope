add_library(parser
     parser.cpp
     parser.h
)

target_link_libraries(parser PRIVATE ast lexer logger)