# implot3d
include_guard(GLOBAL)

find_package(imgui REQUIRED)

FetchContent_Declare(implot3d
  URL "https://github.com/brenocq/implot3d/archive/v0.3.tar.gz"
  URL_HASH SHA256=8f0012043ea4ea56cb5ca9fc44731ad005637f0d3515a5ed3bead27f3096fb55
  DOWNLOAD_NO_PROGRESS ON
  SOURCE_SUBDIR non-existing-path-to-prevent-add-subdirectory
  EXCLUDE_FROM_ALL
  SYSTEM)
message(STATUS "Fetch implot3d ...")
FetchContent_MakeAvailable(implot3d)
file(COPY ${CMAKE_SOURCE_DIR}/cmake/deps/implot3d/CMakeLists.txt DESTINATION ${implot3d_SOURCE_DIR})
add_subdirectory(${implot3d_SOURCE_DIR} ${implot3d_BINARY_DIR} EXCLUDE_FROM_ALL SYSTEM)
set_target_properties(implot3d PROPERTIES FOLDER libs)
mark_as_advanced(FORCE
  FETCHCONTENT_SOURCE_DIR_IMPLOT3D
  FETCHCONTENT_UPDATES_DISCONNECTED_IMPLOT3D)
register_copyright(implot3d "ImPlot3D" "${implot3d_SOURCE_DIR}/LICENSE")
