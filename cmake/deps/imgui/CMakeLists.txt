cmake_minimum_required(VERSION 3.28...3.31 FATAL_ERROR)

project(imgui)

set(IMGUI_PUBLIC_HEADERS
  imconfig.h
  imgui.h)
set(IMGUI_PRIVATE_HEADERS
  imgui_internal.h
  imstb_rectpack.h
  imstb_textedit.h
  imstb_truetype.h
  backends/imgui_impl_opengl3.h
  backends/imgui_impl_opengl3_loader.h
  backends/imgui_impl_glfw.h
  misc/cpp/imgui_stdlib.h
  misc/freetype/imgui_freetype.h)
set(IMGUI_SOURCES
  imgui.cpp
  imgui_demo.cpp
  imgui_draw.cpp
  imgui_tables.cpp
  imgui_widgets.cpp
  backends/imgui_impl_opengl3.cpp
  backends/imgui_impl_glfw.cpp
  misc/cpp/imgui_stdlib.cpp
  misc/freetype/imgui_freetype.cpp)

add_library(imgui STATIC ${IMGUI_SOURCES} ${IMGUI_PUBLIC_HEADERS} ${IMGUI_PRIVATE_HEADERS})
target_compile_definitions(imgui
  PUBLIC
    IMGUI_DISABLE_OBSOLETE_FUNCTIONS
    IMGUI_DISABLE_OBSOLETE_KEYIO # TODO will be part of IMGUI_DISABLE_OBSOLETE_FUNCTIONS in future version
    IMGUI_ENABLE_FREETYPE)
target_include_directories(imgui PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/backends>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/misc/cpp>)
set_target_properties(imgui PROPERTIES
  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
