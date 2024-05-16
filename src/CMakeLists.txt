set(PROJECT_NAME automata)

add_executable(${PROJECT_NAME} main.cpp)
  
add_subdirectory(http)
add_subdirectory(tcp)
add_subdirectory(http_parser)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    http
    tcp
    http_parser
  )