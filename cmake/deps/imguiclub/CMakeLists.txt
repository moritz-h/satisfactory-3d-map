cmake_minimum_required(VERSION 3.28...3.31 FATAL_ERROR)

project(imguiclub)

add_library(imguiclub INTERFACE)
target_include_directories(imguiclub INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/imgui_memory_editor>)
