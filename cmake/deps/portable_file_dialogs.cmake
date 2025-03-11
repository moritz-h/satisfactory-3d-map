# portable_file_dialogs
include_guard(GLOBAL)

FetchContent_Declare(portable_file_dialogs
  URL "https://github.com/samhocevar/portable-file-dialogs/archive/7f852d88a480020d7f91957cbcefe514fc95000c.tar.gz"
  URL_HASH SHA256=69000b1dc01c818c06a72dd428b437eba2d192cd0c7d17cc8d12bd6ca40d8297
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
