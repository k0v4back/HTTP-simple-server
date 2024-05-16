set(PROJECT_NAME
        http
)

project(${PROJECT_NAME})

set(SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/http.cpp
)

set(HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/http.h
)

# add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})