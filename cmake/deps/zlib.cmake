# zlib
include_guard(GLOBAL)

FetchContent_Declare(zlib
  URL "https://github.com/madler/zlib/archive/v1.3.1.tar.gz"
  URL_HASH SHA256=17e88863f3600672ab49182f217281b6fc4d3c762bde361935e436a95214d05c
  SOURCE_SUBDIR non-existing-path-to-prevent-add-subdirectory
  EXCLUDE_FROM_ALL
  SYSTEM)
message(STATUS "Fetch zlib ...")
FetchContent_MakeAvailable(zlib)
file(COPY ${CMAKE_SOURCE_DIR}/cmake/deps/zlib/CMakeLists.txt DESTINATION ${zlib_SOURCE_DIR})
add_subdirectory(${zlib_SOURCE_DIR} ${zlib_BINARY_DIR} EXCLUDE_FROM_ALL SYSTEM)
set_target_properties(zlibstatic PROPERTIES FOLDER libs)
if (SATISFACTORY3DMAP_BUILD_PIC)
  set_target_properties(zlibstatic PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif ()
mark_as_advanced(FORCE
  FETCHCONTENT_SOURCE_DIR_ZLIB
  FETCHCONTENT_UPDATES_DISCONNECTED_ZLIB)
register_copyright(zlib "zlib" "${zlib_SOURCE_DIR}/LICENSE")
