cmake_minimum_required(VERSION 3.21...3.24 FATAL_ERROR)

# Disable in source build
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# Project
project(Satisfactory3DMap
  VERSION 0.5.2
  LANGUAGES CXX)

# Set a default build type if none was specified
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif ()

# Modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)
include(build_info)
include(copyright)

# Dependencies
find_package(OpenMP REQUIRED)
include(libs/libs.cmake)

# Save Library
add_subdirectory(libsave)

# Main App
add_subdirectory(map)
