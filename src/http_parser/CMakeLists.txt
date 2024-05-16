add_library(http_parser "")

target_sources(http_parser
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/http_parser.cpp
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/http_parser.h
  )

target_include_directories(http_parser
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
  )