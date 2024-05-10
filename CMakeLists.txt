cmake_minimum_required(VERSION 3.22)

set(PROJECT_NAME
        http_server
)

project(${PROJECT_NAME})

set(SOURCES
        src/tcp.cpp
        src/http_parser/response.cpp
        src/http_parser/http_parser.cpp
        src/http.cpp
        src/main.cpp
)

set(HEADERS
        src/http_parser/http_parser.h
        src/http_parser/response.h
        src/http.h
        src/tcp.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
set_target_properties(
        ${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
)

target_include_directories(
        ${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}libs/Thread-pool
)

add_subdirectory(libs/Thread-pool)

target_link_libraries(${PROJECT_NAME} ThreadPool)