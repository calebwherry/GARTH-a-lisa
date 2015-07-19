add_library(Canvas Canvas.cpp)
target_link_libraries(Canvas Boost.Compute LodePNG)

add_executable(garth garth.cpp)
target_link_libraries(garth Boost.Compute Canvas)
