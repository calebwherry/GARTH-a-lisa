add_library(Canvas Canvas.cpp Triangle.cpp)
target_link_libraries(Canvas LodePNG)

add_executable(garth garth.cpp)
target_link_libraries(garth OpenCL Canvas)
