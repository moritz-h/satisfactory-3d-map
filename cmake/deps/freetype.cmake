# freetype
include_guard(GLOBAL)

FetchContent_Declare(freetype
  URL "https://github.com/freetype/freetype/archive/VER-2-14-3.tar.gz"
  URL_HASH SHA256=dc49de6b01a266eef4876a4dd34d9842c475d3e28ff2eff63bd2fb760ab56261
  DOWNLOAD_NO_PROGRESS ON
  EXCLUDE_FROM_ALL
  SYSTEM)
message(STATUS "Fetch freetype ...")
option(FT_DISABLE_ZLIB "" ON)
option(FT_DISABLE_BZIP2 "" ON)
option(FT_DISABLE_PNG "" ON)
option(FT_DISABLE_HARFBUZZ "" ON)
option(FT_DISABLE_BROTLI "" ON)
set(SKIP_INSTALL_ALL TRUE)
FetchContent_MakeAvailable(freetype)
unset(SKIP_INSTALL_ALL)
set_target_properties(freetype PROPERTIES
  FOLDER libs
  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
mark_as_advanced(FORCE
  FETCHCONTENT_SOURCE_DIR_FREETYPE
  FETCHCONTENT_UPDATES_DISCONNECTED_FREETYPE
  FT_DISABLE_ZLIB
  FT_DISABLE_BZIP2
  FT_DISABLE_PNG
  FT_DISABLE_HARFBUZZ
  FT_DISABLE_BROTLI
  FT_ENABLE_ERROR_STRINGS)
register_copyright(freetype "FreeType" "${freetype_SOURCE_DIR}/LICENSE.TXT")
