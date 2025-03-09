# imgui

FetchContent_Declare(imgui
  URL "https://github.com/ocornut/imgui/archive/v1.91.8-docking.tar.gz"
  URL_HASH SHA256=55f5e65abea635f2a8bfa9a92cd966448a363a262cf6dead7cc662fb0ab37612)
FetchContent_GetProperties(imgui)
if (NOT imgui_POPULATED)
  message(STATUS "Fetch imgui ...")
  FetchContent_Populate(imgui)
  file(COPY ${CMAKE_SOURCE_DIR}/cmake/deps/imgui/CMakeLists.txt DESTINATION ${imgui_SOURCE_DIR})
  add_subdirectory(${imgui_SOURCE_DIR} ${imgui_BINARY_DIR} EXCLUDE_FROM_ALL)
  target_link_libraries(imgui PRIVATE glfw freetype)
  set_target_properties(imgui PROPERTIES FOLDER libs)
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_IMGUI
    FETCHCONTENT_UPDATES_DISCONNECTED_IMGUI)
  register_copyright(imgui "Dear ImGui" "${imgui_SOURCE_DIR}/LICENSE.txt")
endif ()
