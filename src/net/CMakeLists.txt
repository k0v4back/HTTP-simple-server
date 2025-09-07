cmake_minimum_required(VERSION 3.22)

set(NET_PKT_SOURCES
    src/*.cpp
)

file(GLOB NET_PKT_HEADERS
    include/*.h
)

add_library(net_pkt STATIC ${NET_PKT_SOURCES})