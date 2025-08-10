cmake_minimum_required(VERSION 3.22)

project(http_server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
        src/tcp.cpp
        src/http_parser/response.cpp
        src/http_parser/http_parser.cpp
        src/http.cpp
        src/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(
        ${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}libs/Thread-pool
)

add_subdirectory(libs/Thread-pool)

target_link_libraries(${PROJECT_NAME} ThreadPool_lib)