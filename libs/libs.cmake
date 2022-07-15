# Dependencies

# Setup FetchContent
include(FetchContent)
mark_as_advanced(FORCE
  FETCHCONTENT_BASE_DIR
  FETCHCONTENT_FULLY_DISCONNECTED
  FETCHCONTENT_QUIET
  FETCHCONTENT_UPDATES_DISCONNECTED)

# Resource compiler
FetchContent_Declare(cmrc
  GIT_REPOSITORY https://github.com/vector-of-bool/cmrc.git
  GIT_TAG e386a629eb537d384811e598a3c96b9ca928f65e)
FetchContent_GetProperties(cmrc)
if (NOT cmrc_POPULATED)
  message(STATUS "Fetch cmrc ...")
  FetchContent_Populate(cmrc)
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_CMRC
    FETCHCONTENT_UPDATES_DISCONNECTED_CMRC)
  include(${cmrc_SOURCE_DIR}/CMakeRC.cmake)
  register_library_license("CMakeRC" "${cmrc_SOURCE_DIR}/LICENSE.txt")
endif ()

# spdlog
FetchContent_Declare(spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.10.0)
FetchContent_GetProperties(spdlog)
if (NOT spdlog_POPULATED)
  message(STATUS "Fetch spdlog ...")
  FetchContent_Populate(spdlog)
  option(SPDLOG_DISABLE_DEFAULT_LOGGER "" ON)
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_SPDLOG
    FETCHCONTENT_UPDATES_DISCONNECTED_SPDLOG
    SPDLOG_BUILD_ALL
    SPDLOG_BUILD_BENCH
    SPDLOG_BUILD_EXAMPLE
    SPDLOG_BUILD_EXAMPLE_HO
    SPDLOG_BUILD_SHARED
    SPDLOG_BUILD_TESTS
    SPDLOG_BUILD_TESTS_HO
    SPDLOG_BUILD_WARNINGS
    SPDLOG_CLOCK_COARSE
    SPDLOG_DISABLE_DEFAULT_LOGGER
    SPDLOG_ENABLE_PCH
    SPDLOG_FMT_EXTERNAL
    SPDLOG_FMT_EXTERNAL_HO
    SPDLOG_INSTALL
    SPDLOG_NO_ATOMIC_LEVELS
    SPDLOG_NO_EXCEPTIONS
    SPDLOG_NO_THREAD_ID
    SPDLOG_NO_TLS
    SPDLOG_PREVENT_CHILD_FD
    SPDLOG_SANITIZE_ADDRESS
    SPDLOG_TIDY
    SPDLOG_USE_STD_FORMAT
    SPDLOG_WCHAR_FILENAMES
    SPDLOG_WCHAR_SUPPORT)
  add_subdirectory(${spdlog_SOURCE_DIR} ${spdlog_BINARY_DIR} EXCLUDE_FROM_ALL)
  set_target_properties(spdlog PROPERTIES
    FOLDER libs
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  register_library_license("spdlog" "${spdlog_SOURCE_DIR}/LICENSE")
endif ()

# zlib
FetchContent_Declare(zlib
  GIT_REPOSITORY https://github.com/madler/zlib.git
  GIT_TAG v1.2.12)
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
  register_library_license("zlib" "${CMAKE_SOURCE_DIR}/libs/zlib/LICENSE.txt")
endif ()

# glad2
add_subdirectory(${CMAKE_SOURCE_DIR}/libs/glad/ EXCLUDE_FROM_ALL)
set_target_properties(glad PROPERTIES FOLDER libs)

# GLFW
FetchContent_Declare(glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.3.7)
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
  set_target_properties(glfw PROPERTIES FOLDER libs)
  register_library_license("GLFW" "${glfw_SOURCE_DIR}/LICENSE.md")
endif ()

# glm
FetchContent_Declare(glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 0.9.9.8)
FetchContent_GetProperties(glm)
if (NOT glm_POPULATED)
  message(STATUS "Fetch glm ...")
  FetchContent_Populate(glm)
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_GLM
    FETCHCONTENT_UPDATES_DISCONNECTED_GLM)
  add_subdirectory(${glm_SOURCE_DIR} ${glm_BINARY_DIR} EXCLUDE_FROM_ALL)
  register_library_license("GLM" "${glm_SOURCE_DIR}/copying.txt")
endif ()

# freetype
FetchContent_Declare(freetype
  GIT_REPOSITORY https://github.com/freetype/freetype.git
  GIT_TAG VER-2-12-1)
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
  set(SKIP_INSTALL_ALL TRUE)
  add_subdirectory(${freetype_SOURCE_DIR} ${freetype_BINARY_DIR} EXCLUDE_FROM_ALL)
  unset(SKIP_INSTALL_ALL)
  # VER-2-12-0 has linking conflicts with zlib, because it uses an internal copy of zlib which defines the same symbol
  # names, see https://gitlab.freedesktop.org/freetype/freetype/-/issues/1146 .
  # We can make freetype using our zlib version by still setting FT_DISABLE_ZLIB to skip the find_package an CMake
  # level, but then manually defining the compile definition FT_CONFIG_OPTION_SYSTEM_ZLIB to make the code think it
  # will use a system wide zlib. But instead we can link here to our cmake target.
  target_compile_definitions(freetype PRIVATE FT_CONFIG_OPTION_SYSTEM_ZLIB)
  target_link_libraries(freetype PRIVATE zlibstatic)
  set_target_properties(freetype PROPERTIES
    FOLDER libs
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  register_library_license("FreeType" "${freetype_SOURCE_DIR}/LICENSE.TXT")
endif ()

# imgui
FetchContent_Declare(imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG 9cd9c2eff99877a3f10a7f9c2a3a5b9c15ea36c6) # docking branch after v1.88
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
  register_library_license("Dear ImGui" "${imgui_SOURCE_DIR}/LICENSE.txt")
endif ()

# imguiclub
FetchContent_Declare(imguiclub
  GIT_REPOSITORY https://github.com/ocornut/imgui_club.git
  GIT_TAG d4cd9896e15a03e92702a578586c3f91bbde01e8)
FetchContent_GetProperties(imguiclub)
if (NOT imguiclub_POPULATED)
  message(STATUS "Fetch imguiclub ...")
  FetchContent_Populate(imguiclub)
  file(COPY ${CMAKE_SOURCE_DIR}/libs/imguiclub/CMakeLists.txt DESTINATION ${imguiclub_SOURCE_DIR})
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_IMGUICLUB
    FETCHCONTENT_UPDATES_DISCONNECTED_IMGUICLUB)
  add_subdirectory(${imguiclub_SOURCE_DIR} ${imguiclub_BINARY_DIR} EXCLUDE_FROM_ALL)
  register_library_license("imgui_memory_editor" "${imguiclub_SOURCE_DIR}/LICENSE.txt")
endif ()

# json
FetchContent_Declare(json
  URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz)
FetchContent_GetProperties(json)
if (NOT json_POPULATED)
  message(STATUS "Fetch json ...")
  FetchContent_Populate(json)
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_JSON
    FETCHCONTENT_UPDATES_DISCONNECTED_JSON
    JSON_BuildTests
    JSON_CI
    JSON_Diagnostics
    JSON_ImplicitConversions
    JSON_Install
    JSON_MultipleHeaders
    JSON_SystemInclude)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
  register_library_license("JSON for Modern C++" "${json_SOURCE_DIR}/LICENSE.MIT")
endif ()

# tinygltf
FetchContent_Declare(tinygltf
  GIT_REPOSITORY https://github.com/syoyo/tinygltf.git
  GIT_TAG 544969b7324cd6bba29f6203c7d78c7ea92dbab0)
FetchContent_GetProperties(tinygltf)
if (NOT tinygltf_POPULATED)
  message(STATUS "Fetch tinygltf ...")
  FetchContent_Populate(tinygltf)
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_TINYGLTF
    FETCHCONTENT_UPDATES_DISCONNECTED_TINYGLTF)
  # Build a custom tinygltf version without using the internal json.hpp. Link our json target instead.
  # Further, the current tinygltf CMake does not work well with add_subdirectory and building as static library.
  file(COPY ${tinygltf_SOURCE_DIR}/stb_image.h DESTINATION ${tinygltf_BINARY_DIR}/src/include)
  file(COPY ${tinygltf_SOURCE_DIR}/stb_image_write.h DESTINATION ${tinygltf_BINARY_DIR}/src/include)
  file(COPY ${tinygltf_SOURCE_DIR}/tiny_gltf.h DESTINATION ${tinygltf_BINARY_DIR}/src/include)
  file(COPY ${tinygltf_SOURCE_DIR}/tiny_gltf.cc DESTINATION ${tinygltf_BINARY_DIR}/src/src)
  file(COPY ${CMAKE_SOURCE_DIR}/libs/tinygltf/CMakeLists.txt DESTINATION ${tinygltf_BINARY_DIR}/src)
  add_subdirectory(${tinygltf_BINARY_DIR}/src ${tinygltf_BINARY_DIR}/build EXCLUDE_FROM_ALL)
  set_target_properties(tinygltf PROPERTIES FOLDER libs)
  register_library_license("TinyGLTF" "${tinygltf_SOURCE_DIR}/LICENSE")
endif ()

# glowl
FetchContent_Declare(glowl
  GIT_REPOSITORY https://github.com/invor/glowl.git
  GIT_TAG v0.4g)
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
  register_library_license("glOwl" "${glowl_SOURCE_DIR}/LICENSE")
endif ()

# portable_file_dialogs
FetchContent_Declare(portable_file_dialogs
  GIT_REPOSITORY https://github.com/samhocevar/portable-file-dialogs.git
  GIT_TAG 5652fbd0df05f001aa2e92d86c22f762a03c1fd9)
FetchContent_GetProperties(portable_file_dialogs)
if (NOT portable_file_dialogs_POPULATED)
  message(STATUS "Fetch portable_file_dialogs ...")
  FetchContent_Populate(portable_file_dialogs)
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_PORTABLE_FILE_DIALOGS
    FETCHCONTENT_UPDATES_DISCONNECTED_PORTABLE_FILE_DIALOGS)
  add_subdirectory(${portable_file_dialogs_SOURCE_DIR} ${portable_file_dialogs_BINARY_DIR} EXCLUDE_FROM_ALL)
  register_library_license("Portable File Dialogs" "${portable_file_dialogs_SOURCE_DIR}/COPYING")
endif ()

# iconfontcppheaders
FetchContent_Declare(iconfontcppheaders
  GIT_REPOSITORY https://github.com/juliettef/IconFontCppHeaders.git
  GIT_TAG 7d6ff1f4ba51e7a2b142be39457768abece1549c)
FetchContent_GetProperties(iconfontcppheaders)
if (NOT iconfontcppheaders_POPULATED)
  message(STATUS "Fetch iconfontcppheaders ...")
  FetchContent_Populate(iconfontcppheaders)
  file(COPY ${CMAKE_SOURCE_DIR}/libs/iconfontcppheaders/CMakeLists.txt DESTINATION ${iconfontcppheaders_SOURCE_DIR})
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_ICONFONTCPPHEADERS
    FETCHCONTENT_UPDATES_DISCONNECTED_ICONFONTCPPHEADERS)
  add_subdirectory(${iconfontcppheaders_SOURCE_DIR} ${iconfontcppheaders_BINARY_DIR} EXCLUDE_FROM_ALL)
  register_library_license("IconFontCppHeaders" "${iconfontcppheaders_SOURCE_DIR}/licence.txt")
endif ()

# vdf
FetchContent_Declare(vdf
  GIT_REPOSITORY https://github.com/TinyTinni/ValveFileVDF.git
  GIT_TAG 808be2cd3fc3df6260752f9097aa2731b996f050)
FetchContent_GetProperties(vdf)
if (NOT vdf_POPULATED)
  message(STATUS "Fetch vdf ...")
  FetchContent_Populate(vdf)
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_VDF
    FETCHCONTENT_UPDATES_DISCONNECTED_VDF)
  file(COPY ${vdf_SOURCE_DIR}/vdf_parser.hpp DESTINATION ${vdf_BINARY_DIR}/src)
  file(COPY ${CMAKE_SOURCE_DIR}/libs/vdf/CMakeLists.txt DESTINATION ${vdf_BINARY_DIR}/src)
  add_subdirectory(${vdf_BINARY_DIR}/src ${vdf_BINARY_DIR}/build EXCLUDE_FROM_ALL)
  register_library_license("ValveFileVDF" "${vdf_SOURCE_DIR}/LICENSE")
endif ()
