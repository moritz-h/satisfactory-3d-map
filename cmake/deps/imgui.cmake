# imgui
include_guard(GLOBAL)

find_package(GLFW REQUIRED)
find_package(freetype REQUIRED)

FetchContent_Declare(imgui
  URL "https://github.com/ocornut/imgui/archive/v1.92.2b-docking.tar.gz"
  URL_HASH SHA256=f6ad86e6f938fdda4d5e362b9a9b39158963dd3257fdc9902efc148c0c0c39f9
  DOWNLOAD_NO_PROGRESS ON
  SOURCE_SUBDIR non-existing-path-to-prevent-add-subdirectory
  EXCLUDE_FROM_ALL
  SYSTEM)
message(STATUS "Fetch imgui ...")
FetchContent_MakeAvailable(imgui)
file(COPY ${CMAKE_SOURCE_DIR}/cmake/deps/imgui/CMakeLists.txt DESTINATION ${imgui_SOURCE_DIR})
add_subdirectory(${imgui_SOURCE_DIR} ${imgui_BINARY_DIR} EXCLUDE_FROM_ALL SYSTEM)
target_link_libraries(imgui PRIVATE glfw freetype)
set_target_properties(imgui PROPERTIES FOLDER libs)
mark_as_advanced(FORCE
  FETCHCONTENT_SOURCE_DIR_IMGUI
  FETCHCONTENT_UPDATES_DISCONNECTED_IMGUI)
register_copyright(imgui "Dear ImGui" "${imgui_SOURCE_DIR}/LICENSE.txt")
