# imguiclub

find_package(imgui REQUIRED)

FetchContent_Declare(imguiclub
  URL "https://github.com/ocornut/imgui_club/archive/17510b03602c8b6c7ed48fd8fa29d9a75eb10dc6.tar.gz"
  URL_HASH SHA256=21ef4e6ea2a34b03b54925df96dbbd1ee16e20e0be7ef981085fdb23715aa0b6)
FetchContent_GetProperties(imguiclub)
if (NOT imguiclub_POPULATED)
  message(STATUS "Fetch imguiclub ...")
  FetchContent_Populate(imguiclub)
  file(COPY ${CMAKE_SOURCE_DIR}/cmake/deps/imguiclub/CMakeLists.txt DESTINATION ${imguiclub_SOURCE_DIR})
  add_subdirectory(${imguiclub_SOURCE_DIR} ${imguiclub_BINARY_DIR} EXCLUDE_FROM_ALL)
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_IMGUICLUB
    FETCHCONTENT_UPDATES_DISCONNECTED_IMGUICLUB)
  register_copyright(imguiclub "imgui_memory_editor" "${imguiclub_SOURCE_DIR}/LICENSE.txt")
endif ()
