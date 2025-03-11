cmake_minimum_required(VERSION 3.28...3.31 FATAL_ERROR)

project(SatisfactorySavePy
  LANGUAGES CXX)

find_package(pybind11 REQUIRED)
find_package(spdlog REQUIRED)

file(GLOB_RECURSE code_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

pybind11_add_module(SatisfactorySavePy ${code_files})
target_compile_features(SatisfactorySavePy PUBLIC cxx_std_20)
set_target_properties(SatisfactorySavePy PROPERTIES
  CXX_EXTENSIONS OFF
  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
  OUTPUT_NAME "satisfactory_save")
target_include_directories(SatisfactorySavePy PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)
target_link_libraries(SatisfactorySavePy PRIVATE
  spdlog::spdlog
  SatisfactorySave)

# Tell MSVC all source files are UTF-8 encoded
target_compile_options(SatisfactorySavePy PRIVATE "$<$<OR:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>:/utf-8>")

install(TARGETS SatisfactorySavePy
  LIBRARY DESTINATION .)
