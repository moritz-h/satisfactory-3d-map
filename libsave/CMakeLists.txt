# Save Library

# Project files
file(GLOB_RECURSE public_header_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

file(GLOB_RECURSE code_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")

# Targets
add_library(SatisfactorySave OBJECT)
target_compile_features(SatisfactorySave PUBLIC cxx_std_17)
set_target_properties(SatisfactorySave PROPERTIES
  CXX_EXTENSIONS OFF
  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
target_include_directories(SatisfactorySave
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SatisfactorySave
    ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(SatisfactorySave PRIVATE
  spdlog::spdlog
  zlibstatic
  OpenMP::OpenMP_CXX)
target_sources(SatisfactorySave
  PRIVATE
    ${public_header_files}
    ${code_files})

# Tell MSVC all source files are UTF-8 encoded
target_compile_options(SatisfactorySave PRIVATE "$<$<OR:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>:/utf-8>")

# Visual Studio file tree
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/include" PREFIX include FILES ${public_header_files})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX src FILES ${code_files})

# Warning level
set_warning_level(SatisfactorySave)
