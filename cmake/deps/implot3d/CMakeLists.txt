cmake_minimum_required(VERSION 3.28...4.1 FATAL_ERROR)

project(implot3d)

set(IMPLOT3D_PUBLIC_HEADERS
  implot3d.h)
set(IMPLOT3D_PRIVATE_HEADERS
  implot3d_internal.h)
set(IMPLOT3D_SOURCES
  implot3d.cpp
  implot3d_demo.cpp
  implot3d_items.cpp
  implot3d_meshes.cpp)

add_library(implot3d STATIC ${IMPLOT3D_SOURCES} ${IMPLOT3D_PUBLIC_HEADERS} ${IMPLOT3D_PRIVATE_HEADERS})
target_include_directories(implot3d PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(implot3d PUBLIC imgui)
set_target_properties(implot3d PROPERTIES
  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
