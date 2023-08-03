cmake_minimum_required(VERSION 3.21...3.26)

project(SatisfactorySavePy
  LANGUAGES CXX)

pybind11_add_module(SatisfactorySavePy libsavepy.cpp)
target_compile_features(SatisfactorySavePy PUBLIC cxx_std_17)
set_target_properties(SatisfactorySavePy PROPERTIES
  CXX_EXTENSIONS OFF
  OUTPUT_NAME "satisfactory_save")

# Tell MSVC all source files are UTF-8 encoded
target_compile_options(SatisfactorySavePy PRIVATE "$<$<OR:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>:/utf-8>")

install(TARGETS SatisfactorySavePy
  LIBRARY DESTINATION .)
