# glowl
include_guard(GLOBAL)

find_package(glad REQUIRED)

FetchContent_Declare(glowl
  URL "https://github.com/invor/glowl/archive/e075724a649bd1d57e464d9432556fb69be22699.tar.gz"
  URL_HASH SHA256=aa4d556d4942105d5159a098cceb2845ac173fb80bda240de164f11e88d08f05
  EXCLUDE_FROM_ALL
  SYSTEM)
message(STATUS "Fetch glowl ...")
set(GLOWL_OPENGL_INCLUDE "GLAD2" CACHE STRING "" FORCE)
option(GLOWL_USE_ARB_BINDLESS_TEXTURE "" OFF)
FetchContent_MakeAvailable(glowl)
mark_as_advanced(FORCE
  FETCHCONTENT_SOURCE_DIR_GLOWL
  FETCHCONTENT_UPDATES_DISCONNECTED_GLOWL
  GLOWL_OPENGL_INCLUDE
  GLOWL_USE_ARB_BINDLESS_TEXTURE
  GLOWL_USE_GLM
  GLOWL_USE_NV_MESH_SHADER)
register_copyright(glowl "glOwl" "${glowl_SOURCE_DIR}/LICENSE")
