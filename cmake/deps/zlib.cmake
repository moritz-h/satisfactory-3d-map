# zlib

FetchContent_Declare(zlib
  URL "https://github.com/madler/zlib/archive/v1.3.1.tar.gz"
  URL_HASH SHA256=17e88863f3600672ab49182f217281b6fc4d3c762bde361935e436a95214d05c)
FetchContent_GetProperties(zlib)
if (NOT zlib_POPULATED)
  message(STATUS "Fetch zlib ...")
  FetchContent_Populate(zlib)
  file(COPY ${CMAKE_SOURCE_DIR}/cmake/deps/zlib/CMakeLists.txt DESTINATION ${zlib_SOURCE_DIR})
  add_subdirectory(${zlib_SOURCE_DIR} ${zlib_BINARY_DIR} EXCLUDE_FROM_ALL)
  set_target_properties(zlibstatic PROPERTIES FOLDER libs)
  if (SATISFACTORY3DMAP_BUILD_PIC)
    set_target_properties(zlibstatic PROPERTIES POSITION_INDEPENDENT_CODE ON)
  endif ()
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_ZLIB
    FETCHCONTENT_UPDATES_DISCONNECTED_ZLIB)
  register_copyright(zlib "zlib" "${CMAKE_SOURCE_DIR}/cmake/deps/zlib/LICENSE.txt")
endif ()
