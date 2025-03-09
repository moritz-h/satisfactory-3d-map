# CMake Resource compiler

FetchContent_Declare(cmrc
  URL "https://github.com/vector-of-bool/cmrc/archive/952ffddba731fc110bd50409e8d2b8a06abbd237.tar.gz"
  URL_HASH SHA256=9a1e00d18da46d9765ba5a59f7275dea8b87b242b8885588b3a4694bd4d19747)
FetchContent_GetProperties(cmrc)
if (NOT cmrc_POPULATED)
  message(STATUS "Fetch cmrc ...")
  FetchContent_Populate(cmrc)
  include(${cmrc_SOURCE_DIR}/CMakeRC.cmake)
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_CMRC
    FETCHCONTENT_UPDATES_DISCONNECTED_CMRC)
  register_copyright(cmrc "CMakeRC" "${cmrc_SOURCE_DIR}/LICENSE.txt")
endif ()
