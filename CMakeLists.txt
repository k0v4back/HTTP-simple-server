cmake_minimum_required(VERSION 3.22)
project(http_server VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
endif()

# Specifies debug and release builds.
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0") # Flags for Debug: enable debug information, no optimizations
set(CMAKE_CXX_FLAGS_RELEASE "-O3")  # Flags for Release: Maximum Optimization

# Make sure the CMAKE_BUILD_TYPE variable is set (defaults to Debug if not specified)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build: Debug or Release" FORCE)
endif()

set(SOURCES
        src/http_parser/src/http_response.cpp
        src/http_parser/src/http_parser.cpp
        src/http.cpp
        src/net/src/tcp.cpp
        src/config_parser/src/config_parser.cpp
        main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(
        ${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}libs/Thread-pool
)

add_subdirectory(libs/Thread-pool)

target_link_libraries(${PROJECT_NAME} ThreadPool_lib)