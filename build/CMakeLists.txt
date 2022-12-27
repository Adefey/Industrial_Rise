cmake_minimum_required(VERSION 3.24)

project(industrial_rise)

FILE(GLOB Sources ../sources/*.cpp)

add_executable(industrial_map_reduce ${Sources})

target_include_directories(industrial_map_reduce PUBLIC ../include)
