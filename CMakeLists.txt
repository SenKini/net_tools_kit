cmake_minimum_required(VERSION 3.25)
project(network_tool_kit)

set(CMAKE_CXX_STANDARD 20)

file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(kit ${SRC})