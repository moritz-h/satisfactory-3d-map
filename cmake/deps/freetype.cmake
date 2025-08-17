# freetype
include_guard(GLOBAL)

FetchContent_Declare(freetype
  URL "https://github.com/freetype/freetype/archive/VER-2-13-3.tar.gz"
  URL_HASH SHA256=bc5c898e4756d373e0d991bab053036c5eb2aa7c0d5c67e8662ddc6da40c4103
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
SET(CMAKE_POLICY_VERSION_MINIMUM 3.10) # Remove with next version
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
