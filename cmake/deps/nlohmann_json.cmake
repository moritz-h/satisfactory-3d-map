# json
include_guard(GLOBAL)

FetchContent_Declare(json
  URL "https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz"
  URL_HASH SHA256=42f6e95cad6ec532fd372391373363b62a14af6d771056dbfc86160e6dfff7aa
  DOWNLOAD_NO_PROGRESS ON
  EXCLUDE_FROM_ALL
  SYSTEM)
message(STATUS "Fetch json ...")
FetchContent_MakeAvailable(json)
mark_as_advanced(FORCE
  FETCHCONTENT_SOURCE_DIR_JSON
  FETCHCONTENT_UPDATES_DISCONNECTED_JSON
  JSON_BuildTests
  JSON_CI
  JSON_Diagnostic_Positions
  JSON_Diagnostics
  JSON_DisableEnumSerialization
  JSON_GlobalUDLs
  JSON_ImplicitConversions
  JSON_Install
  JSON_LegacyDiscardedValueComparison
  JSON_MultipleHeaders
  JSON_SystemInclude)
register_copyright(json "JSON for Modern C++" "${json_SOURCE_DIR}/LICENSE.MIT")
