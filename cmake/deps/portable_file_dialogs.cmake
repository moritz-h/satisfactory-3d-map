# portable_file_dialogs
include_guard(GLOBAL)

FetchContent_Declare(portable_file_dialogs
  URL "https://github.com/samhocevar/portable-file-dialogs/archive/c12ea8c9a727f5320a2b4570aee863bbede2a204.tar.gz"
  URL_HASH SHA256=8f06b35c6017e7e6796cf7393815f7ccde2268a9259d553573da78e2a405b1b2
  DOWNLOAD_NO_PROGRESS ON
  SOURCE_SUBDIR non-existing-path-to-prevent-add-subdirectory
  EXCLUDE_FROM_ALL
  SYSTEM)
message(STATUS "Fetch portable_file_dialogs ...")
FetchContent_MakeAvailable(portable_file_dialogs)
file(COPY ${CMAKE_SOURCE_DIR}/cmake/deps/portable_file_dialogs/CMakeLists.txt DESTINATION ${portable_file_dialogs_SOURCE_DIR})
add_subdirectory(${portable_file_dialogs_SOURCE_DIR} ${portable_file_dialogs_BINARY_DIR} EXCLUDE_FROM_ALL SYSTEM)
mark_as_advanced(FORCE
  FETCHCONTENT_SOURCE_DIR_PORTABLE_FILE_DIALOGS
  FETCHCONTENT_UPDATES_DISCONNECTED_PORTABLE_FILE_DIALOGS)
register_copyright(portable_file_dialogs "Portable File Dialogs" "${portable_file_dialogs_SOURCE_DIR}/COPYING")
