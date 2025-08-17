# iconfontcppheaders
include_guard(GLOBAL)

FetchContent_Declare(iconfontcppheaders
  URL "https://github.com/juliettef/IconFontCppHeaders/archive/1c004c59fea885bab18897fb8387df10c7d9307f.tar.gz"
  URL_HASH SHA256=bb5171c544bc18a42e84ba5ccb842426dbb8fa6dfe91ff632e0c8fdce13bc912
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
