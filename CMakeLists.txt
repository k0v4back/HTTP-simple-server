cmake_minimum_required(VERSION 3.22)

set(PROJECT_NAME http_server)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
  ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
  ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
  ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

add_subdirectory(src)
add_subdirectory(libs/Thread-pool)

# enable_testing()
# add_subdirectory(tests)



# set(SOURCES
#         src/tcp.cpp
#         src/http_parser/response.cpp
#         src/http_parser/http_parser.cpp
#         src/http.cpp
#         src/main.cpp
# )

# set(HEADERS
#         src/http_parser/http_parser.h
#         src/http_parser/response.h
#         src/http.h
#         src/tcp.h
# )

# add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# target_include_directories(
#         ${PROJECT_NAME}
#         PRIVATE
#         ${CMAKE_CURRENT_SOURCE_DIR}libs/Thread-pool
# )

# add_subdirectory(libs/Thread-pool)

# target_link_libraries(${PROJECT_NAME} ThreadPool)