cmake_minimum_required(VERSION 3.21...3.29 FATAL_ERROR)

project(imguiclub)

add_library(imguiclub INTERFACE)
target_include_directories(imguiclub INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/imgui_memory_editor>)
