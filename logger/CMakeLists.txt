add_library(logger 
    logger.cpp
    logger.h
)

target_include_directories(logger PUBLIC ${CMAKE_SOURCE_DIR}/ast)

target_link_libraries(logger PRIVATE ast)