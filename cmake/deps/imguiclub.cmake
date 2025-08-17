# imguiclub
include_guard(GLOBAL)

find_package(imgui REQUIRED)

FetchContent_Declare(imguiclub
  URL "https://github.com/ocornut/imgui_club/archive/019ed6171a5b46448eb8a2c8e7e8a1d792c722fd.tar.gz"
  URL_HASH SHA256=cd003517fc645bafcf2e13316a150debc1934f9e200aae55a1b9b2366c58cdec
  DOWNLOAD_NO_PROGRESS ON
  SOURCE_SUBDIR non-existing-path-to-prevent-add-subdirectory
  EXCLUDE_FROM_ALL
  SYSTEM)
message(STATUS "Fetch imguiclub ...")
FetchContent_MakeAvailable(imguiclub)
file(COPY ${CMAKE_SOURCE_DIR}/cmake/deps/imguiclub/CMakeLists.txt DESTINATION ${imguiclub_SOURCE_DIR})
add_subdirectory(${imguiclub_SOURCE_DIR} ${imguiclub_BINARY_DIR} EXCLUDE_FROM_ALL SYSTEM)
mark_as_advanced(FORCE
  FETCHCONTENT_SOURCE_DIR_IMGUICLUB
  FETCHCONTENT_UPDATES_DISCONNECTED_IMGUICLUB)
register_copyright(imguiclub "imgui_memory_editor" "${imguiclub_SOURCE_DIR}/LICENSE.txt")
