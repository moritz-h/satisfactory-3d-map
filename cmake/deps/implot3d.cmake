# implot3d
include_guard(GLOBAL)

find_package(imgui REQUIRED)

FetchContent_Declare(implot3d
  URL "https://github.com/brenocq/implot3d/archive/8e70e1e2dd838efdc34498706faa5ceab7f61ba1.tar.gz"
  URL_HASH SHA256=81f6b7a8544d06f2ae7e3872471f7c05bc384c16f9e43d47c3427e2ccccc7853
  DOWNLOAD_NO_PROGRESS ON
  SOURCE_SUBDIR non-existing-path-to-prevent-add-subdirectory
  EXCLUDE_FROM_ALL
  SYSTEM)
message(STATUS "Fetch implot3d ...")
FetchContent_MakeAvailable(implot3d)
file(COPY ${CMAKE_SOURCE_DIR}/cmake/deps/implot3d/CMakeLists.txt DESTINATION ${implot3d_SOURCE_DIR})
add_subdirectory(${implot3d_SOURCE_DIR} ${implot3d_BINARY_DIR} EXCLUDE_FROM_ALL SYSTEM)
mark_as_advanced(FORCE
  FETCHCONTENT_SOURCE_DIR_IMPLOT3D
  FETCHCONTENT_UPDATES_DISCONNECTED_IMPLOT3D)
register_copyright(implot3d "ImPlot3D" "${implot3d_SOURCE_DIR}/LICENSE")
