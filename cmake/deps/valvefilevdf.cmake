# vdf

FetchContent_Declare(vdf
  URL "https://github.com/TinyTinni/ValveFileVDF/archive/97a021ce2b7ab3e950f6b0a4f373ef7becbafe15.tar.gz"
  URL_HASH SHA256=04001c0ad97f9ccbcfd416c8b3ceb0f71604b1af894db89031a490192e4d8ee2)
FetchContent_GetProperties(vdf)
if (NOT vdf_POPULATED)
  message(STATUS "Fetch vdf ...")
  FetchContent_Populate(vdf)
  add_subdirectory(${vdf_SOURCE_DIR} ${vdf_BINARY_DIR} EXCLUDE_FROM_ALL)
  set_target_properties(ValveFileVDF PROPERTIES FOLDER libs)
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_VDF
    FETCHCONTENT_UPDATES_DISCONNECTED_VDF
    VALVEFILEVDF_ENABLE_FUZZING
    VALVEFILEVDF_ENABLE_PYTHON
    VALVEFILEVDF_ENABLE_TESTING)
  register_copyright(vdf "ValveFileVDF" "${vdf_SOURCE_DIR}/LICENSE")
endif ()
