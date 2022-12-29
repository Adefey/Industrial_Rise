cmake_minimum_required(VERSION 3.18)

project(industrial_rise_test)

set (CMAKE_CXX_STANDARD 20)

find_package(GTest REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fPIC")

FILE(GLOB Sources ../../../sources/industrial*.cpp ../main.cpp)

add_executable(industrial_test ${Sources})

set_property(SOURCE ${Sources} APPEND PROPERTY COMPILE_OPTIONS "-coverage")

target_include_directories(industrial_test PUBLIC ../../../include  ${GTEST_INCLUDE_DIRS})

target_link_libraries(industrial_test PUBLIC ${GTEST_LIBRARIES} pthread gcov gtest)
