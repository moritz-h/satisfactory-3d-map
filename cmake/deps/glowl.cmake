# glowl

find_package(glad REQUIRED)

FetchContent_Declare(glowl
  URL "https://github.com/invor/glowl/archive/e075724a649bd1d57e464d9432556fb69be22699.tar.gz"
  URL_HASH SHA256=aa4d556d4942105d5159a098cceb2845ac173fb80bda240de164f11e88d08f05)
FetchContent_GetProperties(glowl)
if (NOT glowl_POPULATED)
  message(STATUS "Fetch glowl ...")
  FetchContent_Populate(glowl)
  set(GLOWL_OPENGL_INCLUDE "GLAD2" CACHE STRING "" FORCE)
  option(GLOWL_USE_ARB_BINDLESS_TEXTURE "" OFF)
  add_subdirectory(${glowl_SOURCE_DIR} ${glowl_BINARY_DIR} EXCLUDE_FROM_ALL)
  if (NOT WIN32)
    # Mark include dirs as 'system' to disable warnings.
    get_target_property(include_dirs glowl INTERFACE_INCLUDE_DIRECTORIES)
    set_target_properties(glowl PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES "${include_dirs}")
  endif ()
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_GLOWL
    FETCHCONTENT_UPDATES_DISCONNECTED_GLOWL
    GLOWL_OPENGL_INCLUDE
    GLOWL_USE_ARB_BINDLESS_TEXTURE
    GLOWL_USE_GLM
    GLOWL_USE_NV_MESH_SHADER)
  register_copyright(glowl "glOwl" "${glowl_SOURCE_DIR}/LICENSE")
endif ()
