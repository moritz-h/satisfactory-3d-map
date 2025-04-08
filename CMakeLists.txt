cmake_minimum_required(VERSION 3.28...3.31 FATAL_ERROR)

# Disable in source build
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# Dependency Provider
option(SATISFACTORY3DMAP_DEPENDENCY_PROVIDER "Use internal dependency provider." ON)
if (SATISFACTORY3DMAP_DEPENDENCY_PROVIDER)
  list(PREPEND CMAKE_PROJECT_TOP_LEVEL_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/cmake/dependency_provider.cmake")
endif ()

# Project
project(Satisfactory3DMap
  VERSION 0.9.0
  LANGUAGES CXX)

# Set a default build type if none was specified
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif ()

# Modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)
include(build_info)
include(copyright)
include(warning_level)

# Options
option(SATISFACTORY3DMAP_INSTALL_LIB "Install SatisfactorySave library." OFF)
option(SATISFACTORY3DMAP_BUILD_SHARED "Build shared library." OFF)
option(SATISFACTORY3DMAP_BUILD_PIC "Build position independent code." ON)
option(SATISFACTORY3DMAP_BUILD_APP "Build Satisfactory3DMap App." ON)
option(SATISFACTORY3DMAP_BUILD_LIB_PYTHON "Build SatisfactorySave Python library." OFF)
option(SATISFACTORY3DMAP_STRICT_WARNINGS "Enable strict compiler warnings." OFF)
option(SATISFACTORY3DMAP_BENCHMARK "Benchmark save loading." OFF)

# Save Library
add_subdirectory(libsave)

# Main App
if (SATISFACTORY3DMAP_BUILD_APP)
  add_subdirectory(map)
  if (MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
  endif ()
endif ()

# Python Library
if (SATISFACTORY3DMAP_BUILD_LIB_PYTHON)
  add_subdirectory(libsavepy)
endif ()

# Visual Studio target folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
