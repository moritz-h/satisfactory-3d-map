# vdf
include_guard(GLOBAL)

FetchContent_Declare(vdf
  URL "https://github.com/TinyTinni/ValveFileVDF/archive/v1.1.1.tar.gz"
  URL_HASH SHA256=de16a199c535c3b49f2aa0bd17e3154e02b32fa7b0949053ba6d981f8c32197f
  EXCLUDE_FROM_ALL
  SYSTEM)
message(STATUS "Fetch vdf ...")
FetchContent_MakeAvailable(vdf)
set_target_properties(ValveFileVDF PROPERTIES FOLDER libs)
mark_as_advanced(FORCE
  FETCHCONTENT_SOURCE_DIR_VDF
  FETCHCONTENT_UPDATES_DISCONNECTED_VDF
  VALVEFILEVDF_ENABLE_FUZZING
  VALVEFILEVDF_ENABLE_PYTHON
  VALVEFILEVDF_ENABLE_TESTING)
register_copyright(vdf "ValveFileVDF" "${vdf_SOURCE_DIR}/LICENSE")
