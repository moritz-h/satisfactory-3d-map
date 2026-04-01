# iconfontcppheaders
include_guard(GLOBAL)

FetchContent_Declare(iconfontcppheaders
  URL "https://github.com/juliettef/IconFontCppHeaders/archive/3ee7f3d295ae773c0046db8d7b89b886eb2526de.tar.gz"
  URL_HASH SHA256=77b450aa8e67c0e6c120caa1a2ac3b842e861427e79c89e1400d2a18d575c31b
  DOWNLOAD_NO_PROGRESS ON
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
