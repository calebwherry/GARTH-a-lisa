########################################
## \file   CMakeLists.txt
## \date   04/11/2015
## \brief  Main fractals project CMake file.
########################################


#
# Minimum CMake version:
#
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)


#
# Project Properties:
#
project(GARTH-a-lisa CXX C)
set(${PROJECT_NAME}_MAJOR_VERSION 0)
set(${PROJECT_NAME}_MINOR_VERSION 1)
set(${PROJECT_NAME}_PATCH_VERSION 0)
set(${PROJECT_NAME}_VERSION
  ${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}.${${PROJECT_NAME}_PATCH_VERSION}
)


#
# Override some variables if not set: 
#

# Default build type:
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Default build type." FORCE)
endif()

# Shared libs:
if (NOT DEFINED BUILD_SHARED_LIBS)
  set(BUILD_SHARED_LIBS ON CACHE STRING "Build libraries as shared." FORCE)
endif()


#
# Print useful CMake info:
#
message(STATUS "Project Version: ${${PROJECT_NAME}_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Shared libs: ${BUILD_SHARED_LIBS}")


#
# Add local module path:
#
set(
  CMAKE_MODULE_PATH 
  "${PROJECT_SOURCE_DIR}/cmake" 
  ${CMAKE_MODULE_PATH}
)


#
# Project-wide compiler options:
#
include(compiler_options)


#
# Add subdirectories:
#
add_subdirectory(ext)
add_subdirectory(src)
