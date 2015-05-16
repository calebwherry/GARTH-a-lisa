add_library(Canvas Canvas.cpp)
target_link_libraries(Canvas)

add_executable(garth garth.cpp)
target_link_libraries(garth OpenCL Canvas)
