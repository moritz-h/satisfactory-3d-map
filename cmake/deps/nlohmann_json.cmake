# json
include_guard(GLOBAL)

FetchContent_Declare(json
  URL "https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz"
  URL_HASH SHA256=d6c65aca6b1ed68e7a182f4757257b107ae403032760ed6ef121c9d55e81757d
  EXCLUDE_FROM_ALL
  SYSTEM)
message(STATUS "Fetch json ...")
FetchContent_MakeAvailable(json)
mark_as_advanced(FORCE
  FETCHCONTENT_SOURCE_DIR_JSON
  FETCHCONTENT_UPDATES_DISCONNECTED_JSON
  JSON_BuildTests
  JSON_CI
  JSON_Diagnostics
  JSON_DisableEnumSerialization
  JSON_GlobalUDLs
  JSON_ImplicitConversions
  JSON_Install
  JSON_LegacyDiscardedValueComparison
  JSON_MultipleHeaders
  JSON_SystemInclude)
register_copyright(json "JSON for Modern C++" "${json_SOURCE_DIR}/LICENSE.MIT")
