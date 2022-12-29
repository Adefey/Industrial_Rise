cmake_minimum_required(VERSION 3.18)

project(industrial_rise)

set (CMAKE_CXX_STANDARD 20)

FILE(GLOB Sources ../sources/*.cpp)

add_executable(industrial_map_reduce ${Sources})

target_include_directories(industrial_map_reduce PUBLIC ../include)

target_link_libraries(industrial_map_reduce PUBLIC pthread )
