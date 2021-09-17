cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

# Policies
cmake_policy(SET CMP0091 NEW)

# Disable in source build
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# Project
project(Satisfactory3DMap
  VERSION 0.1.0
  LANGUAGES CXX)

# Set a default build type if none was specified
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif ()

# Dependencies
find_package(OpenMP REQUIRED)
include(libs/libs.cmake)

# Project files
file(GLOB_RECURSE code_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")

# Resource files
file(GLOB_RECURSE resource_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  "${CMAKE_CURRENT_SOURCE_DIR}/resources/*")
cmrc_add_resource_library(resources WHENCE resources ${resource_files})
set_target_properties(resources PROPERTIES
  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
  FOLDER libs)

# Disable extra console window on Windows.
set(WIN32_FLAG "")
if (MSVC)
  option(SATISFACTORY3DMAP_DISABLE_CONSOLE "Disable extra console window." OFF)
  if (SATISFACTORY3DMAP_DISABLE_CONSOLE)
    set(WIN32_FLAG "WIN32")
  endif ()
endif (MSVC)

# Targets
add_executable(${PROJECT_NAME} ${WIN32_FLAG})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_EXTENSIONS OFF
  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME} PRIVATE
  zlibstatic
  glad
  glfw
  glm
  imgui
  imguiclub
  tinygltf
  glowl
  resources
  OpenMP::OpenMP_CXX)
if (MSVC AND SATISFACTORY3DMAP_DISABLE_CONSOLE)
  target_link_options(${PROJECT_NAME} PRIVATE "/entry:mainCRTStartup")
endif ()
target_sources(${PROJECT_NAME} PRIVATE ${code_files})

# Tell MSVC all source files are UTF-8 encoded
target_compile_options(${PROJECT_NAME} PRIVATE "$<$<OR:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>:/utf-8>")

# Visual Studio file tree
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX src FILES ${code_files})

# Visual Studio target folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
if (MSVC)
  set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif ()

# Install
include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# Package
set(SATISFACTORY3DMAP_CPACK_GENERATOR "ZIP" CACHE STRING "Choose CPack generator.")
if (UNIX)
  set_property(CACHE SATISFACTORY3DMAP_CPACK_GENERATOR PROPERTY STRINGS "TGZ" "ZIP" "STGZ" "DEB")
else ()
  set_property(CACHE SATISFACTORY3DMAP_CPACK_GENERATOR PROPERTY STRINGS "ZIP" "NSIS")
endif ()

set(CPACK_GENERATOR "${SATISFACTORY3DMAP_CPACK_GENERATOR}")
set(CPACK_PACKAGE_VENDOR "Satisfactory3DMap")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Satisfactory 3D Map.")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

if (SATISFACTORY3DMAP_CPACK_GENERATOR STREQUAL "DEB")
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Satisfactory3DMap")
  set(CPACK_DEBIAN_FILE_NAME "DEB-DEFAULT")
  set(CPACK_DEBIAN_PACKAGE_DEPENDS "")
  # https://stackoverflow.com/a/7363073
  set(CPACK_SET_DESTDIR true)
  set(CPACK_INSTALL_PREFIX /opt/satisfactory3dmap)
elseif (SATISFACTORY3DMAP_CPACK_GENERATOR STREQUAL "NSIS")
  # Start menu entry
  set(CPACK_PACKAGE_EXECUTABLES "Satisfactory3DMap;Satisfactory3DMap")
endif ()

# TODO: configure source packages

include(CPack)

# Enable strict warnings
option(SATISFACTORY3DMAP_STRICT_WARNINGS "Enable strict compiler warnings." OFF)
if (SATISFACTORY3DMAP_STRICT_WARNINGS)
  if (MSVC)
    # TODO: For now do not set "/WX" flag, as external libs generate warnings, which we cannot fix.
    # MSVC has an experimental feature to disable external warnings, see https://devblogs.microsoft.com/cppblog/broken-warnings-theory/.
    # Maybe in future there is cmake support for this feature, see https://gitlab.kitware.com/cmake/cmake/-/issues/17904.
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
  else ()
    target_compile_options(${PROJECT_NAME} PRIVATE -Werror -Wall -Wextra -pedantic -pedantic-errors)
  endif ()
endif ()
