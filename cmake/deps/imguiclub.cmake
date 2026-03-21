# imguiclub
include_guard(GLOBAL)

find_package(imgui REQUIRED)

FetchContent_Declare(imguiclub
  URL "https://github.com/ocornut/imgui_club/archive/1e7facddfd50ba9ce4b75477b111d611c439076e.tar.gz"
  URL_HASH SHA256=77a135652c4cdbb533b9865b3b391aade9f4839c4ba2b52e1f9f11b06ce05564
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
