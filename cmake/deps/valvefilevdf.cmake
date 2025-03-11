# vdf
include_guard(GLOBAL)

FetchContent_Declare(vdf
  URL "https://github.com/TinyTinni/ValveFileVDF/archive/97a021ce2b7ab3e950f6b0a4f373ef7becbafe15.tar.gz"
  URL_HASH SHA256=04001c0ad97f9ccbcfd416c8b3ceb0f71604b1af894db89031a490192e4d8ee2
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
