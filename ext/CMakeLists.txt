########################################
## \file   CMakeLists.txt
## \date   04/19/2015
## \brief  lodepng-level CMake file.
########################################

include(ExternalProject)

# Add external project:
set(LodePNGSourcePath "${CMAKE_CURRENT_BINARY_DIR}/lodepng/src/LodePNGExt/")
ExternalProject_Add(
  LodePNGExt
  PREFIX            "${CMAKE_CURRENT_BINARY_DIR}/lodepng"
  GIT_REPOSITORY    "https://github.com/lvandeve/lodepng.git"
  CONFIGURE_COMMAND ""
  BUILD_COMMAND     ""
  INSTALL_COMMAND   ""
)

# 'Dummy' command to make sure above step is done before library creation below:
#   Note: Otherwise CMake will try and look for the source files before
#         they are downloaded.
add_custom_command(
  COMMAND ""
  DEPENDS LodePNGExt
  OUTPUT  "${LodePNGSourcePath}/lodepng.h" "${LodePNGSourcePath}/lodepng.cpp"
)

# Create CMake library:
add_library(
  LodePNG
  "${LodePNGSourcePath}/lodepng.cpp"
)
target_include_directories(LodePNG PUBLIC "${LodePNGSourcePath}")
add_dependencies(LodePNG LodePNGExt)
