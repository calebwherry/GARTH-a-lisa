cmake_minimum_required(VERSION 3.0)

project(LodePNG CXX)

add_library(LodePNG lodepng.cpp lodepng.h)
target_include_directories(LodePNG PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
