# Dependency Provider

# Setup FetchContent
include(FetchContent)
mark_as_advanced(FORCE
  FETCHCONTENT_BASE_DIR
  FETCHCONTENT_FULLY_DISCONNECTED
  FETCHCONTENT_QUIET
  FETCHCONTENT_UPDATES_DISCONNECTED)

set(deps_call_stack "")

macro(satisfactory3dmap_provide_dependency method dep_name)
  if ("${dep_name}" IN_LIST deps_call_stack)
    list(JOIN deps_call_stack " => " deps_call_stack_print)
    message(FATAL_ERROR "Cyclic dependency found in dependency provider: ${deps_call_stack_print} => ${dep_name}")
  endif ()
  list(APPEND deps_call_stack "${dep_name}")
  string(TOLOWER ${dep_name} dep_name_lower)
  set(dep_file "${CMAKE_SOURCE_DIR}/cmake/deps/${dep_name_lower}.cmake")
  if (EXISTS "${dep_file}")
    include("${dep_file}")
    set(${dep_name}_FOUND TRUE)
  endif ()
  list(POP_BACK deps_call_stack)
endmacro()

cmake_language(
  SET_DEPENDENCY_PROVIDER satisfactory3dmap_provide_dependency
  SUPPORTED_METHODS FIND_PACKAGE
)
