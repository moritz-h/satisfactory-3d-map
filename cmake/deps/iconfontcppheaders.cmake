# iconfontcppheaders
include_guard(GLOBAL)

FetchContent_Declare(iconfontcppheaders
  URL "https://github.com/juliettef/IconFontCppHeaders/archive/68d0f7cd3b61547c10326029a8a83051ba49b348.tar.gz"
  URL_HASH SHA256=b6cf988330d767930dc15ae37d6a2e2833f8fc040771df4667e2f1cf0e1f931e
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
