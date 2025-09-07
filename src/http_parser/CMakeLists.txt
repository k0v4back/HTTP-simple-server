cmake_minimum_required(VERSION 3.22)

set(HTTP_PARSER_SOURCES
    src/*.cpp
)

file(GLOB HTTP_PARSER_HEADERS
    include/*.h
)

add_library(http_parser STATIC ${HTTP_PARSER_SOURCES})