add_library(tcp "")

target_sources(tcp
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/tcp.cpp
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/tcp.h
  )

target_include_directories(tcp
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
  )