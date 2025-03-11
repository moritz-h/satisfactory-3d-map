# natsort
include_guard(GLOBAL)

FetchContent_Declare(natsort
  URL "https://github.com/sourcefrog/natsort/archive/cdd8df9602e727482ae5e051cff74b7ec7ffa07a.tar.gz"
  URL_HASH SHA256=5b24abd4011ffe4754839a4f2648ed6dd64c8b28c95836bcecc5c7dc868108af
  SOURCE_SUBDIR non-existing-path-to-prevent-add-subdirectory
  EXCLUDE_FROM_ALL
  SYSTEM)
message(STATUS "Fetch natsort ...")
FetchContent_MakeAvailable(natsort)
file(COPY ${CMAKE_SOURCE_DIR}/cmake/deps/natsort/CMakeLists.txt DESTINATION ${natsort_SOURCE_DIR})
add_subdirectory(${natsort_SOURCE_DIR} ${natsort_BINARY_DIR} EXCLUDE_FROM_ALL SYSTEM)
set_target_properties(natsort PROPERTIES FOLDER libs)
if (SATISFACTORY3DMAP_BUILD_PIC)
  set_target_properties(natsort PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif ()
mark_as_advanced(FORCE
  FETCHCONTENT_SOURCE_DIR_NATSORT
  FETCHCONTENT_UPDATES_DISCONNECTED_NATSORT)
register_copyright(natsort "natsort" "${CMAKE_SOURCE_DIR}/cmake/deps/natsort/LICENSE.txt")
