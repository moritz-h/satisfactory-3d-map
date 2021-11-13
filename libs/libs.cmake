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
  GIT_TAG        a64bea50c05594c8e7cf1f08e441bb9507742e2e)
FetchContent_GetProperties(cmrm)
if (NOT cmrm_POPULATED)
  message(STATUS "Fetch cmrm ...")
  FetchContent_Populate(cmrm)
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_CMRM
    FETCHCONTENT_UPDATES_DISCONNECTED_CMRM)
  include(${cmrm_SOURCE_DIR}/CMakeRC.cmake)
endif ()

# zlib
FetchContent_Declare(zlib
  GIT_REPOSITORY https://github.com/madler/zlib.git
  GIT_TAG        v1.2.11)
FetchContent_GetProperties(zlib)
if (NOT zlib_POPULATED)
  message(STATUS "Fetch zlib ...")
  FetchContent_Populate(zlib)
  file(COPY ${CMAKE_SOURCE_DIR}/libs/zlib/CMakeLists.txt DESTINATION ${zlib_SOURCE_DIR})
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_ZLIB
    FETCHCONTENT_UPDATES_DISCONNECTED_ZLIB)
  add_subdirectory(${zlib_SOURCE_DIR} ${zlib_BINARY_DIR} EXCLUDE_FROM_ALL)
  set_target_properties(zlibstatic PROPERTIES FOLDER libs)
endif ()

# glad2
add_subdirectory(${CMAKE_SOURCE_DIR}/libs/glad/)
set_target_properties(glad PROPERTIES FOLDER libs)

# GLFW
FetchContent_Declare(glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG        3.3.4)
FetchContent_GetProperties(glfw)
if (NOT glfw_POPULATED)
  message(STATUS "Fetch glfw ...")
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
  message(STATUS "Fetch glm ...")
  FetchContent_Populate(glm)
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_GLM
    FETCHCONTENT_UPDATES_DISCONNECTED_GLM)
  add_subdirectory(${glm_SOURCE_DIR} ${glm_BINARY_DIR} EXCLUDE_FROM_ALL)
endif ()

# freetype
FetchContent_Declare(freetype
  GIT_REPOSITORY https://github.com/freetype/freetype.git
  GIT_TAG        94cb3a2eb96b3f17a1a3bd0e6f7da97c0e1d8f57) # Latest release VER-2-11-0 is missing disable options
FetchContent_GetProperties(freetype)
if (NOT freetype_POPULATED)
  message(STATUS "Fetch freetype ...")
  FetchContent_Populate(freetype)
  option(FT_DISABLE_ZLIB "" ON)
  option(FT_DISABLE_BZIP2 "" ON)
  option(FT_DISABLE_PNG "" ON)
  option(FT_DISABLE_HARFBUZZ "" ON)
  option(FT_DISABLE_BROTLI "" ON)
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_FREETYPE
    FETCHCONTENT_UPDATES_DISCONNECTED_FREETYPE
    FT_DISABLE_ZLIB
    FT_DISABLE_BZIP2
    FT_DISABLE_PNG
    FT_DISABLE_HARFBUZZ
    FT_DISABLE_BROTLI)
  set(CMAKE_PROJECT_INCLUDE_BEFORE "${CMAKE_SOURCE_DIR}/libs/freetype/DisableWarnings.cmake")
  add_subdirectory(${freetype_SOURCE_DIR} ${freetype_BINARY_DIR} EXCLUDE_FROM_ALL)
  unset(CMAKE_PROJECT_INCLUDE_BEFORE)
  set_target_properties(freetype PROPERTIES FOLDER libs)
endif ()

# imgui
FetchContent_Declare(imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG        1b215ecb018ba0fd170618366ddc4be9bd45f283) # docking branch after v1.85
FetchContent_GetProperties(imgui)
if (NOT imgui_POPULATED)
  message(STATUS "Fetch imgui ...")
  FetchContent_Populate(imgui)
  file(COPY ${CMAKE_SOURCE_DIR}/libs/imgui/CMakeLists.txt DESTINATION ${imgui_SOURCE_DIR})
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_IMGUI
    FETCHCONTENT_UPDATES_DISCONNECTED_IMGUI)
  add_subdirectory(${imgui_SOURCE_DIR} ${imgui_BINARY_DIR} EXCLUDE_FROM_ALL)
  target_link_libraries(imgui PRIVATE glad glfw freetype)
  set_target_properties(imgui PROPERTIES FOLDER libs)
endif ()

# imguiclub
FetchContent_Declare(imguiclub
  GIT_REPOSITORY https://github.com/ocornut/imgui_club.git
  GIT_TAG        02e679b7f4cfb01f9480dcbcac59552879f96573)
FetchContent_GetProperties(imguiclub)
if (NOT imguiclub_POPULATED)
  message(STATUS "Fetch imguiclub ...")
  FetchContent_Populate(imguiclub)
  file(COPY ${CMAKE_SOURCE_DIR}/libs/imguiclub/CMakeLists.txt DESTINATION ${imguiclub_SOURCE_DIR})
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_IMGUICLUB
    FETCHCONTENT_UPDATES_DISCONNECTED_IMGUICLUB)
  add_subdirectory(${imguiclub_SOURCE_DIR} ${imguiclub_BINARY_DIR} EXCLUDE_FROM_ALL)
endif ()

# tinygltf
FetchContent_Declare(tinygltf
  GIT_REPOSITORY https://github.com/syoyo/tinygltf.git
  GIT_TAG        b86eb6c07964e5a35058b8e7a1cc4cdad6afe204)
FetchContent_GetProperties(tinygltf)
if (NOT tinygltf_POPULATED)
  message(STATUS "Fetch tinygltf ...")
  FetchContent_Populate(tinygltf)
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_TINYGLTF
    FETCHCONTENT_UPDATES_DISCONNECTED_TINYGLTF)
  # TODO tinygltf CMake now better supports add_subdirectory, but not as static library yet.
  file(COPY ${tinygltf_SOURCE_DIR}/json.hpp DESTINATION ${tinygltf_BINARY_DIR}/include)
  file(COPY ${tinygltf_SOURCE_DIR}/stb_image.h DESTINATION ${tinygltf_BINARY_DIR}/include)
  file(COPY ${tinygltf_SOURCE_DIR}/stb_image_write.h DESTINATION ${tinygltf_BINARY_DIR}/include)
  file(COPY ${tinygltf_SOURCE_DIR}/tiny_gltf.h DESTINATION ${tinygltf_BINARY_DIR}/include)
  file(COPY ${CMAKE_SOURCE_DIR}/libs/tinygltf/CMakeLists.txt DESTINATION ${tinygltf_BINARY_DIR})
  file(COPY ${CMAKE_SOURCE_DIR}/libs/tinygltf/tiny_gltf.cpp DESTINATION ${tinygltf_BINARY_DIR}/src)
  add_subdirectory(${tinygltf_BINARY_DIR} ${tinygltf_BINARY_DIR} EXCLUDE_FROM_ALL)
  set_target_properties(tinygltf PROPERTIES FOLDER libs)
endif ()

# glowl
FetchContent_Declare(glowl
  GIT_REPOSITORY https://github.com/invor/glowl.git
  GIT_TAG        v0.4g)
FetchContent_GetProperties(glowl)
if (NOT glowl_POPULATED)
  message(STATUS "Fetch glowl ...")
  FetchContent_Populate(glowl)
  set(GLOWL_OPENGL_INCLUDE "GLAD2" CACHE STRING "" FORCE)
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_GLOWL
    FETCHCONTENT_UPDATES_DISCONNECTED_GLOWL
    GLOWL_OPENGL_INCLUDE)
  add_subdirectory(${glowl_SOURCE_DIR} ${glowl_BINARY_DIR} EXCLUDE_FROM_ALL)
  target_compile_definitions(glowl INTERFACE
    GLOWL_NO_ARB_BINDLESS_TEXTURE)
  # Mark include dirs as 'system' to disable warnings.
  get_target_property(include_dirs glowl INTERFACE_INCLUDE_DIRECTORIES)
  set_target_properties(glowl PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES "${include_dirs}")
endif ()

# portable_file_dialogs
FetchContent_Declare(portable_file_dialogs
  GIT_REPOSITORY https://github.com/samhocevar/portable-file-dialogs.git
  GIT_TAG        7a7a9f5fa800b28331672742c0e774d46186e34f)
FetchContent_GetProperties(portable_file_dialogs)
if (NOT portable_file_dialogs_POPULATED)
  message(STATUS "Fetch portable_file_dialogs ...")
  FetchContent_Populate(portable_file_dialogs)
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_PORTABLE_FILE_DIALOGS
    FETCHCONTENT_UPDATES_DISCONNECTED_PORTABLE_FILE_DIALOGS)
  add_subdirectory(${portable_file_dialogs_SOURCE_DIR} ${portable_file_dialogs_BINARY_DIR} EXCLUDE_FROM_ALL)
endif ()

# iconfontcppheaders
FetchContent_Declare(iconfontcppheaders
  GIT_REPOSITORY https://github.com/juliettef/IconFontCppHeaders.git
  GIT_TAG        69a88452ff06e1cbe1a9467c6522a12191936d95)
FetchContent_GetProperties(iconfontcppheaders)
if (NOT iconfontcppheaders_POPULATED)
  message(STATUS "Fetch iconfontcppheaders ...")
  FetchContent_Populate(iconfontcppheaders)
  file(COPY ${CMAKE_SOURCE_DIR}/libs/iconfontcppheaders/CMakeLists.txt DESTINATION ${iconfontcppheaders_SOURCE_DIR})
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_ICONFONTCPPHEADERS
    FETCHCONTENT_UPDATES_DISCONNECTED_ICONFONTCPPHEADERS)
  add_subdirectory(${iconfontcppheaders_SOURCE_DIR} ${iconfontcppheaders_BINARY_DIR} EXCLUDE_FROM_ALL)
endif ()
