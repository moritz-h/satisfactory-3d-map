# Dependency Provider

# Setup FetchContent
include(FetchContent)
mark_as_advanced(FORCE
  FETCHCONTENT_BASE_DIR
  FETCHCONTENT_FULLY_DISCONNECTED
  FETCHCONTENT_QUIET
  FETCHCONTENT_UPDATES_DISCONNECTED)
# TODO
if (POLICY CMP0169)
  cmake_policy(SET CMP0169 OLD)
endif ()

macro(satisfactory3dmap_provide_dependency method dep_name)
  string(TOLOWER ${dep_name} dep_name_lower)
  set(dep_file "${CMAKE_SOURCE_DIR}/cmake/deps/${dep_name_lower}.cmake")
  if (EXISTS "${dep_file}")
    include("${dep_file}")
    set(${dep_name}_FOUND TRUE)
  endif ()
endmacro()

cmake_language(
  SET_DEPENDENCY_PROVIDER satisfactory3dmap_provide_dependency
  SUPPORTED_METHODS FIND_PACKAGE
)
