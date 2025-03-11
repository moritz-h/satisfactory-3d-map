cmake_minimum_required(VERSION 3.28...3.31 FATAL_ERROR)

project(tinygltf)

add_library(tinygltf STATIC src/tiny_gltf.cc)
add_library(tinygltf::tinygltf ALIAS tinygltf)
target_include_directories(tinygltf PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_compile_definitions(tinygltf PRIVATE TINYGLTF_USE_CPP14)
set_target_properties(tinygltf PROPERTIES
  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
target_link_libraries(tinygltf PRIVATE nlohmann_json)

# tinygltf includes just "json.hpp", but nlohmann_json CMake assumes "nlohmann/json.hpp" as include.
# Modify include path to fix this.
get_target_property(JSON_INCLUDE_DIRS nlohmann_json INTERFACE_INCLUDE_DIRECTORIES)
string(REPLACE "json-src/include/" "json-src/include/nlohmann/" JSON_INCLUDE_DIRS "${JSON_INCLUDE_DIRS}")
target_include_directories(tinygltf PRIVATE "${JSON_INCLUDE_DIRS}")
