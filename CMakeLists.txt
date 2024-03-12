cmake_minimum_required(VERSION 3.16.3)
project(solid-require)

set(CMAKE_CXX_STANDARD 17)

add_library(solid-require solid/require.cpp)
target_include_directories(solid-require PUBLIC solid/require.h)
