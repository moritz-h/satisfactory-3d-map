cmake_minimum_required(VERSION 3.28...3.31 FATAL_ERROR)

project(natsort)

add_library(natsort STATIC strnatcmp.c)
target_include_directories(natsort PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
set_target_properties(natsort PROPERTIES
  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
