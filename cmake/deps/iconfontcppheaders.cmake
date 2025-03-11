# iconfontcppheaders
include_guard(GLOBAL)

FetchContent_Declare(iconfontcppheaders
  URL "https://github.com/juliettef/IconFontCppHeaders/archive/ef464d2fe5a568d30d7c88138e78d7fac7cfebc5.tar.gz"
  URL_HASH SHA256=a3535d255d930f646a98f01eaea456d00324e46d6577c119ce01fcc1f4f39af7
  SOURCE_SUBDIR non-existing-path-to-prevent-add-subdirectory
  EXCLUDE_FROM_ALL
  SYSTEM)
message(STATUS "Fetch iconfontcppheaders ...")
FetchContent_MakeAvailable(iconfontcppheaders)
file(COPY ${CMAKE_SOURCE_DIR}/cmake/deps/iconfontcppheaders/CMakeLists.txt DESTINATION ${iconfontcppheaders_SOURCE_DIR})
add_subdirectory(${iconfontcppheaders_SOURCE_DIR} ${iconfontcppheaders_BINARY_DIR} EXCLUDE_FROM_ALL SYSTEM)
mark_as_advanced(FORCE
  FETCHCONTENT_SOURCE_DIR_ICONFONTCPPHEADERS
  FETCHCONTENT_UPDATES_DISCONNECTED_ICONFONTCPPHEADERS)
register_copyright(iconfontcppheaders "IconFontCppHeaders" "${iconfontcppheaders_SOURCE_DIR}/licence.txt")
