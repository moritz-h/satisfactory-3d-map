# Dependencies

# Setup FetchContent
include(FetchContent)
mark_as_advanced(FORCE
  FETCHCONTENT_BASE_DIR
  FETCHCONTENT_FULLY_DISCONNECTED
  FETCHCONTENT_QUIET
  FETCHCONTENT_UPDATES_DISCONNECTED)

# Resource compiler
FetchContent_Declare(cmrm
  GIT_REPOSITORY https://github.com/vector-of-bool/cmrc.git
  GIT_TAG        e7ba9e9417960b2a5cefc9e79e8af8b06bfde3d1)
FetchContent_GetProperties(cmrm)
if (NOT cmrm_POPULATED)
  FetchContent_Populate(cmrm)
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_CMRM
    FETCHCONTENT_UPDATES_DISCONNECTED_CMRM)
  include(${cmrm_SOURCE_DIR}/CMakeRC.cmake)
endif ()

#ZLIB
FetchContent_Declare(zlib
  GIT_REPOSITORY https://github.com/madler/zlib.git
  GIT_TAG        v1.2.11)
FetchContent_GetProperties(zlib)
if (NOT zlib_POPULATED)
  FetchContent_Populate(zlib)
  file(COPY ${CMAKE_SOURCE_DIR}/libs/zlib/CMakeLists.txt DESTINATION ${zlib_SOURCE_DIR})
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_ZLIB
    FETCHCONTENT_UPDATES_DISCONNECTED_ZLIB)
  add_subdirectory(${zlib_SOURCE_DIR} EXCLUDE_FROM_ALL)
  set_target_properties(zlibstatic PROPERTIES FOLDER libs)
endif ()

# glad2
add_subdirectory(${CMAKE_SOURCE_DIR}/libs/glad/)
set_target_properties(glad PROPERTIES FOLDER libs)

# GLFW
FetchContent_Declare(glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG        3.3.3)
FetchContent_GetProperties(glfw)
if (NOT glfw_POPULATED)
  FetchContent_Populate(glfw)
  option(GLFW_BUILD_DOCS "" OFF)
  option(GLFW_BUILD_EXAMPLES "" OFF)
  option(GLFW_BUILD_TESTS "" OFF)
  option(GLFW_INSTALL "" OFF)
  if (WIN32)
    option(GLFW_USE_HYBRID_HPG "" ON)
    option(USE_MSVC_RUNTIME_LIBRARY_DLL "" OFF)
  endif ()
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_GLFW
    FETCHCONTENT_UPDATES_DISCONNECTED_GLFW
    BUILD_SHARED_LIBS
    GLFW_BUILD_DOCS
    GLFW_BUILD_EXAMPLES
    GLFW_BUILD_TESTS
    GLFW_INSTALL
    GLFW_USE_HYBRID_HPG
    GLFW_USE_OSMESA
    GLFW_USE_WAYLAND
    GLFW_VULKAN_STATIC
    USE_MSVC_RUNTIME_LIBRARY_DLL
    X11_xcb_icccm_INCLUDE_PATH
    X11_xcb_icccm_LIB)
  add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR} EXCLUDE_FROM_ALL)
  set_target_properties(glfw PROPERTIES
    FOLDER libs
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  # The GLFW option USE_MSVC_RUNTIME_LIBRARY_DLL uses string replacements for the static linking flags. As CMake 3.15+
  # is not setting default flags, we ned to add the new MSVC_RUNTIME_LIBRARY property manually here for static linking.
endif ()

# glm
FetchContent_Declare(glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG        0.9.9.8)
FetchContent_GetProperties(glm)
if (NOT glm_POPULATED)
  FetchContent_Populate(glm)
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_GLM
    FETCHCONTENT_UPDATES_DISCONNECTED_GLM)
  add_subdirectory(${glm_SOURCE_DIR} ${glm_BINARY_DIR} EXCLUDE_FROM_ALL)
endif ()

# imgui
FetchContent_Declare(imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG        239d09804d17997e147f4bcfb451ead04c1d67ff) # docking branch after v1.81
FetchContent_GetProperties(imgui)
if (NOT imgui_POPULATED)
  FetchContent_Populate(imgui)
  file(COPY ${CMAKE_SOURCE_DIR}/libs/imgui/CMakeLists.txt DESTINATION ${imgui_SOURCE_DIR})
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_IMGUI
    FETCHCONTENT_UPDATES_DISCONNECTED_IMGUI)
  add_subdirectory(${imgui_SOURCE_DIR} ${imgui_BINARY_DIR} EXCLUDE_FROM_ALL)
  target_link_libraries(imgui PRIVATE glad glfw)
  set_target_properties(imgui PROPERTIES FOLDER libs)
endif ()

# tinygltf
FetchContent_Declare(tinygltf
  GIT_REPOSITORY https://github.com/syoyo/tinygltf.git
  GIT_TAG        298c37a9549ae04bb991f0b2c77d270bccc66eb0)
FetchContent_GetProperties(tinygltf)
if (NOT tinygltf_POPULATED)
  FetchContent_Populate(tinygltf)
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_TINYGLTF
    FETCHCONTENT_UPDATES_DISCONNECTED_TINYGLTF)
  file(COPY ${tinygltf_SOURCE_DIR}/json.hpp DESTINATION ${tinygltf_BINARY_DIR}/include)
  file(COPY ${tinygltf_SOURCE_DIR}/stb_image.h DESTINATION ${tinygltf_BINARY_DIR}/include)
  file(COPY ${tinygltf_SOURCE_DIR}/stb_image_write.h DESTINATION ${tinygltf_BINARY_DIR}/include)
  file(COPY ${tinygltf_SOURCE_DIR}/tiny_gltf.h DESTINATION ${tinygltf_BINARY_DIR}/include)
  file(COPY ${CMAKE_SOURCE_DIR}/libs/tinygltf/CMakeLists.txt DESTINATION ${tinygltf_BINARY_DIR})
  file(COPY ${CMAKE_SOURCE_DIR}/libs/tinygltf/tiny_gltf.cpp DESTINATION ${tinygltf_BINARY_DIR}/src)
  add_subdirectory(${tinygltf_BINARY_DIR} EXCLUDE_FROM_ALL)
  set_target_properties(tinygltf PROPERTIES FOLDER libs)
endif ()

# glowl
FetchContent_Declare(glowl
  GIT_REPOSITORY https://github.com/moritz-h/glowl.git
  GIT_TAG        946befd08439f0cc922e0f1cff4035e0d21101dd)
FetchContent_GetProperties(glowl)
if (NOT glowl_POPULATED)
  FetchContent_Populate(glowl)
  set(GLOWL_OPENGL_INCLUDE "GLAD2" CACHE STRING "" FORCE)
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_GLOWL
    FETCHCONTENT_UPDATES_DISCONNECTED_GLOWL
    GLOWL_OPENGL_INCLUDE)
  add_subdirectory(${glowl_SOURCE_DIR} ${glowl_BINARY_DIR} EXCLUDE_FROM_ALL)
  target_compile_definitions(glowl INTERFACE
    GLOWL_NO_ARB_BINDLESS_TEXTURE)
endif ()
