# Save Library

# Dependencies
find_package(OpenMP REQUIRED)
find_package(spdlog REQUIRED)
find_package(zlib REQUIRED)
find_package(natsort REQUIRED)

# Project files
file(GLOB_RECURSE public_header_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

file(GLOB_RECURSE code_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")

# Targets
if (SATISFACTORY3DMAP_BUILD_SHARED)
  add_library(SatisfactorySave SHARED)
else ()
  add_library(SatisfactorySave STATIC)
endif ()
add_library(SatisfactorySave::SatisfactorySave ALIAS SatisfactorySave)
target_compile_features(SatisfactorySave PUBLIC cxx_std_20)
set_target_properties(SatisfactorySave PROPERTIES
  CXX_EXTENSIONS OFF
  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
target_include_directories(SatisfactorySave
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<INSTALL_INTERFACE:include/SatisfactorySave/export>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/export>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SatisfactorySave
    ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(SatisfactorySave PRIVATE
  spdlog::spdlog
  ZLIB::ZLIBSTATIC
  natsort
  OpenMP::OpenMP_CXX)
target_sources(SatisfactorySave
  PRIVATE
    ${public_header_files}
    ${code_files})

include(GenerateExportHeader)
generate_export_header(SatisfactorySave
  EXPORT_MACRO_NAME SATISFACTORYSAVE_API
  EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/export/satisfactorysave_export.h")

if (SATISFACTORY3DMAP_BUILD_PIC)
  set_target_properties(SatisfactorySave PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif ()

# Benchmark
if (SATISFACTORY3DMAP_BENCHMARK)
  target_compile_definitions(SatisfactorySave PRIVATE "MEASURE_TIME")
endif ()

# Tell MSVC all source files are UTF-8 encoded
target_compile_options(SatisfactorySave PRIVATE "$<$<OR:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>:/utf-8>")

# Install
if (SATISFACTORY3DMAP_INSTALL_LIB)
  include(GNUInstallDirs)

  install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/export/satisfactorysave_export.h"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/SatisfactorySave/export)

  install(TARGETS SatisfactorySave
    EXPORT SatisfactorySaveTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

  install(EXPORT SatisfactorySaveTargets
    FILE SatisfactorySaveConfig.cmake
    NAMESPACE SatisfactorySave::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/satisfactorysave)
endif ()

# Visual Studio file tree
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/include" PREFIX include FILES ${public_header_files})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX src FILES ${code_files})

# Warning level
set_warning_level(SatisfactorySave)
