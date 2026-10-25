cmake_minimum_required(VERSION 3.16)

project(HTTPServer
    VERSION 1.0.0
    DESCRIPTION "HTTP Server written in C++"
    LANGUAGES CXX
)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile_commands.json for IDEs and static analysis tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Compiler warnings
add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
)

# Find threads
find_package(Threads REQUIRED)

# Thread pool library
add_subdirectory(libs/Thread-pool)

# HTTP server sources
set(HTTP_SERVER_SOURCES
    main.cpp

    src/config/config_parser.cpp

    src/http/http_parser.cpp
    src/http/http_response.cpp

    src/net/tcp.cpp

    src/server/http_server.cpp
)

# Executable
add_executable(http_server ${HTTP_SERVER_SOURCES})

# Include directories
target_include_directories(http_server
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link libraries
target_link_libraries(http_server
    PRIVATE
        ThreadPool_lib
        Threads::Threads
)

# Runtime configuration
set_target_properties(http_server PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)