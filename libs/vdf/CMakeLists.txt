cmake_minimum_required(VERSION 3.21...3.27 FATAL_ERROR)

project(vdf)

add_library(vdf INTERFACE)
target_include_directories(vdf INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
