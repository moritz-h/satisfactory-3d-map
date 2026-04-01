# imguiclub
include_guard(GLOBAL)

find_package(imgui REQUIRED)

FetchContent_Declare(imguiclub
  URL "https://github.com/ocornut/imgui_club/archive/a7eab6ccb9fec09f37705406a06bb3bfc09597fe.tar.gz"
  URL_HASH SHA256=b3dd70ac5e273b5f9e9e3691c1e1620427d13703cc3ce73c11cd39b52c863bc8
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
