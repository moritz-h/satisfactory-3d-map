cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

cmake_policy(SET CMP0091 NEW)

project(tinygltf)

add_library(tinygltf STATIC src/tiny_gltf.cpp)
target_include_directories(tinygltf PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_compile_definitions(tinygltf PRIVATE TINYGLTF_USE_CPP14)
set_target_properties(tinygltf PROPERTIES
  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
