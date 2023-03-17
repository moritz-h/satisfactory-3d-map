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
  URL "https://github.com/vector-of-bool/cmrc/archive/9a3396444e0478bd6f261075e74d1ecf70964029.tar.gz"
  URL_HASH SHA256=96f9653880a685f7796b7c0b380a1d6a4ed1d6d6996fd49e61d5c30462734dba)
FetchContent_GetProperties(cmrc)
if (NOT cmrc_POPULATED)
  message(STATUS "Fetch cmrc ...")
  FetchContent_Populate(cmrc)
  include(${cmrc_SOURCE_DIR}/CMakeRC.cmake)
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_CMRC
    FETCHCONTENT_UPDATES_DISCONNECTED_CMRC)
  register_library_copyright("CMakeRC" "${cmrc_SOURCE_DIR}/LICENSE.txt")
endif ()

# spdlog
FetchContent_Declare(spdlog
  URL "https://github.com/gabime/spdlog/archive/v1.11.0.tar.gz"
  URL_HASH SHA256=ca5cae8d6cac15dae0ec63b21d6ad3530070650f68076f3a4a862ca293a858bb)
FetchContent_GetProperties(spdlog)
if (NOT spdlog_POPULATED)
  message(STATUS "Fetch spdlog ...")
  FetchContent_Populate(spdlog)
  option(SPDLOG_DISABLE_DEFAULT_LOGGER "" ON)
  add_subdirectory(${spdlog_SOURCE_DIR} ${spdlog_BINARY_DIR} EXCLUDE_FROM_ALL)
  set_target_properties(spdlog PROPERTIES
    FOLDER libs
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_SPDLOG
    FETCHCONTENT_UPDATES_DISCONNECTED_SPDLOG
    SPDLOG_BUILD_ALL
    SPDLOG_BUILD_BENCH
    SPDLOG_BUILD_EXAMPLE
    SPDLOG_BUILD_EXAMPLE_HO
    SPDLOG_BUILD_PIC
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
  register_library_copyright("spdlog" "${spdlog_SOURCE_DIR}/LICENSE")
endif ()

# zlib
FetchContent_Declare(zlib
  URL "https://github.com/madler/zlib/archive/v1.2.13.tar.gz"
  URL_HASH SHA256=1525952a0a567581792613a9723333d7f8cc20b87a81f920fb8bc7e3f2251428)
FetchContent_GetProperties(zlib)
if (NOT zlib_POPULATED)
  message(STATUS "Fetch zlib ...")
  FetchContent_Populate(zlib)
  file(COPY ${CMAKE_SOURCE_DIR}/libs/zlib/CMakeLists.txt DESTINATION ${zlib_SOURCE_DIR})
  add_subdirectory(${zlib_SOURCE_DIR} ${zlib_BINARY_DIR} EXCLUDE_FROM_ALL)
  set_target_properties(zlibstatic PROPERTIES FOLDER libs)
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_ZLIB
    FETCHCONTENT_UPDATES_DISCONNECTED_ZLIB)
  register_library_copyright("zlib" "${CMAKE_SOURCE_DIR}/libs/zlib/LICENSE.txt")
endif ()

# glad2
add_subdirectory(${CMAKE_SOURCE_DIR}/libs/glad/ EXCLUDE_FROM_ALL)
set_target_properties(glad PROPERTIES FOLDER libs)
register_library_copyright("Glad 2" "${CMAKE_SOURCE_DIR}/libs/glad/LICENSE")

# GLFW
FetchContent_Declare(glfw
  URL "https://github.com/glfw/glfw/archive/3.3.8.tar.gz"
  URL_HASH SHA256=f30f42e05f11e5fc62483e513b0488d5bceeab7d9c5da0ffe2252ad81816c713)
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
  add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR} EXCLUDE_FROM_ALL)
  set_target_properties(glfw PROPERTIES FOLDER libs)
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
  register_library_copyright("GLFW" "${glfw_SOURCE_DIR}/LICENSE.md")
endif ()

# glm
FetchContent_Declare(glm
  URL "https://github.com/g-truc/glm/archive/0.9.9.8.tar.gz"
  URL_HASH SHA256=7d508ab72cb5d43227a3711420f06ff99b0a0cb63ee2f93631b162bfe1fe9592)
FetchContent_GetProperties(glm)
if (NOT glm_POPULATED)
  message(STATUS "Fetch glm ...")
  FetchContent_Populate(glm)
  add_subdirectory(${glm_SOURCE_DIR} ${glm_BINARY_DIR} EXCLUDE_FROM_ALL)
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_GLM
    FETCHCONTENT_UPDATES_DISCONNECTED_GLM)
  register_library_copyright("GLM" "${glm_SOURCE_DIR}/copying.txt")
endif ()

# freetype
FetchContent_Declare(freetype
  URL "https://github.com/freetype/freetype/archive/VER-2-13-0.tar.gz"
  URL_HASH SHA256=a683f1091aee95d2deaca9292d976f87415610b8ae1ea186abeebcb08e83ab12)
FetchContent_GetProperties(freetype)
if (NOT freetype_POPULATED)
  message(STATUS "Fetch freetype ...")
  FetchContent_Populate(freetype)
  option(FT_DISABLE_ZLIB "" ON)
  option(FT_DISABLE_BZIP2 "" ON)
  option(FT_DISABLE_PNG "" ON)
  option(FT_DISABLE_HARFBUZZ "" ON)
  option(FT_DISABLE_BROTLI "" ON)
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
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_FREETYPE
    FETCHCONTENT_UPDATES_DISCONNECTED_FREETYPE
    FT_DISABLE_ZLIB
    FT_DISABLE_BZIP2
    FT_DISABLE_PNG
    FT_DISABLE_HARFBUZZ
    FT_DISABLE_BROTLI
    FT_ENABLE_ERROR_STRINGS)
  register_library_copyright("FreeType" "${freetype_SOURCE_DIR}/LICENSE.TXT")
endif ()

# imgui
FetchContent_Declare(imgui
  URL "https://github.com/ocornut/imgui/archive/9e30fb0ec1b44dc1b041db6bdd53b130b2a18509.tar.gz" # docking branch after v1.89.4
  URL_HASH SHA256=7ab191bf0a7da273995e0fce145aef9d8cd05a761f3c333b7d4f5811fa012133)
FetchContent_GetProperties(imgui)
if (NOT imgui_POPULATED)
  message(STATUS "Fetch imgui ...")
  FetchContent_Populate(imgui)
  file(COPY ${CMAKE_SOURCE_DIR}/libs/imgui/CMakeLists.txt DESTINATION ${imgui_SOURCE_DIR})
  add_subdirectory(${imgui_SOURCE_DIR} ${imgui_BINARY_DIR} EXCLUDE_FROM_ALL)
  target_link_libraries(imgui PRIVATE glfw freetype)
  set_target_properties(imgui PROPERTIES FOLDER libs)
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_IMGUI
    FETCHCONTENT_UPDATES_DISCONNECTED_IMGUI)
  register_library_copyright("Dear ImGui" "${imgui_SOURCE_DIR}/LICENSE.txt")
endif ()

# imguiclub
FetchContent_Declare(imguiclub
  URL "https://github.com/ocornut/imgui_club/archive/ea49dd3c6803088d50b496e3fe981501543b7cbc.tar.gz"
  URL_HASH SHA256=ccc6e5b95152f4ad0a3229e43d781e7410fa2b4d623c1a76e4a92d5ffca751ac)
FetchContent_GetProperties(imguiclub)
if (NOT imguiclub_POPULATED)
  message(STATUS "Fetch imguiclub ...")
  FetchContent_Populate(imguiclub)
  file(COPY ${CMAKE_SOURCE_DIR}/libs/imguiclub/CMakeLists.txt DESTINATION ${imguiclub_SOURCE_DIR})
  add_subdirectory(${imguiclub_SOURCE_DIR} ${imguiclub_BINARY_DIR} EXCLUDE_FROM_ALL)
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_IMGUICLUB
    FETCHCONTENT_UPDATES_DISCONNECTED_IMGUICLUB)
  register_library_copyright("imgui_memory_editor" "${imguiclub_SOURCE_DIR}/LICENSE.txt")
endif ()

# json
FetchContent_Declare(json
  URL "https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz"
  URL_HASH SHA256=8c4b26bf4b422252e13f332bc5e388ec0ab5c3443d24399acb675e68278d341f)
FetchContent_GetProperties(json)
if (NOT json_POPULATED)
  message(STATUS "Fetch json ...")
  FetchContent_Populate(json)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_JSON
    FETCHCONTENT_UPDATES_DISCONNECTED_JSON
    JSON_BuildTests
    JSON_CI
    JSON_Diagnostics
    JSON_DisableEnumSerialization
    JSON_GlobalUDLs
    JSON_ImplicitConversions
    JSON_Install
    JSON_LegacyDiscardedValueComparison
    JSON_MultipleHeaders
    JSON_SystemInclude)
  register_library_copyright("JSON for Modern C++" "${json_SOURCE_DIR}/LICENSE.MIT")
endif ()

# tinygltf
FetchContent_Declare(tinygltf
  URL "https://github.com/syoyo/tinygltf/archive/v.2.8.4.tar.gz"
  URL_HASH SHA256=1703878830185fce50b801854b86894c3aa320d633d9104d67b6084d3f72ee3b)
FetchContent_GetProperties(tinygltf)
if (NOT tinygltf_POPULATED)
  message(STATUS "Fetch tinygltf ...")
  FetchContent_Populate(tinygltf)
  # Build a custom tinygltf version without using the internal json.hpp. Link our json target instead.
  # Further, the current tinygltf CMake does not work well with add_subdirectory and building as static library.
  file(COPY ${tinygltf_SOURCE_DIR}/stb_image.h DESTINATION ${tinygltf_BINARY_DIR}/src/include)
  file(COPY ${tinygltf_SOURCE_DIR}/stb_image_write.h DESTINATION ${tinygltf_BINARY_DIR}/src/include)
  file(COPY ${tinygltf_SOURCE_DIR}/tiny_gltf.h DESTINATION ${tinygltf_BINARY_DIR}/src/include)
  file(COPY ${tinygltf_SOURCE_DIR}/tiny_gltf.cc DESTINATION ${tinygltf_BINARY_DIR}/src/src)
  file(COPY ${CMAKE_SOURCE_DIR}/libs/tinygltf/CMakeLists.txt DESTINATION ${tinygltf_BINARY_DIR}/src)
  add_subdirectory(${tinygltf_BINARY_DIR}/src ${tinygltf_BINARY_DIR}/build EXCLUDE_FROM_ALL)
  set_target_properties(tinygltf PROPERTIES FOLDER libs)
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_TINYGLTF
    FETCHCONTENT_UPDATES_DISCONNECTED_TINYGLTF)
  register_library_copyright("TinyGLTF" "${tinygltf_SOURCE_DIR}/LICENSE")
endif ()

# glowl
FetchContent_Declare(glowl
  URL "https://github.com/invor/glowl/archive/5511d334bf857c17b4bd1642f4fe25220ab7abf1.tar.gz"
  URL_HASH SHA256=f0acd154c36293adbc9eed26a1c5eaa5bfdbefb93b50beeb241ae7975cd77424)
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
  register_library_copyright("glOwl" "${glowl_SOURCE_DIR}/LICENSE")
endif ()

# portable_file_dialogs
FetchContent_Declare(portable_file_dialogs
  URL "https://github.com/samhocevar/portable-file-dialogs/archive/7f852d88a480020d7f91957cbcefe514fc95000c.tar.gz"
  URL_HASH SHA256=69000b1dc01c818c06a72dd428b437eba2d192cd0c7d17cc8d12bd6ca40d8297)
FetchContent_GetProperties(portable_file_dialogs)
if (NOT portable_file_dialogs_POPULATED)
  message(STATUS "Fetch portable_file_dialogs ...")
  FetchContent_Populate(portable_file_dialogs)
  add_subdirectory(${portable_file_dialogs_SOURCE_DIR} ${portable_file_dialogs_BINARY_DIR} EXCLUDE_FROM_ALL)
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_PORTABLE_FILE_DIALOGS
    FETCHCONTENT_UPDATES_DISCONNECTED_PORTABLE_FILE_DIALOGS)
  register_library_copyright("Portable File Dialogs" "${portable_file_dialogs_SOURCE_DIR}/COPYING")
endif ()

# iconfontcppheaders
FetchContent_Declare(iconfontcppheaders
  URL "https://github.com/juliettef/IconFontCppHeaders/archive/fdeaa7dacef4507cccce0fb1df75b0f7546e15d4.tar.gz"
  URL_HASH SHA256=12c18d0213fb45e5c5403cb2d420a04774b377e2eb8d065cc72d8ef4029e53d3)
FetchContent_GetProperties(iconfontcppheaders)
if (NOT iconfontcppheaders_POPULATED)
  message(STATUS "Fetch iconfontcppheaders ...")
  FetchContent_Populate(iconfontcppheaders)
  file(COPY ${CMAKE_SOURCE_DIR}/libs/iconfontcppheaders/CMakeLists.txt DESTINATION ${iconfontcppheaders_SOURCE_DIR})
  add_subdirectory(${iconfontcppheaders_SOURCE_DIR} ${iconfontcppheaders_BINARY_DIR} EXCLUDE_FROM_ALL)
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_ICONFONTCPPHEADERS
    FETCHCONTENT_UPDATES_DISCONNECTED_ICONFONTCPPHEADERS)
  register_library_copyright("IconFontCppHeaders" "${iconfontcppheaders_SOURCE_DIR}/licence.txt")
endif ()

# vdf
FetchContent_Declare(vdf
  URL "https://github.com/TinyTinni/ValveFileVDF/archive/808be2cd3fc3df6260752f9097aa2731b996f050.tar.gz"
  URL_HASH SHA256=6ed7f1b0dd81b689352cf2d89c54677063fc356743126f06a7f49af4675a67b2)
FetchContent_GetProperties(vdf)
if (NOT vdf_POPULATED)
  message(STATUS "Fetch vdf ...")
  FetchContent_Populate(vdf)
  file(COPY ${vdf_SOURCE_DIR}/vdf_parser.hpp DESTINATION ${vdf_BINARY_DIR}/src)
  file(COPY ${CMAKE_SOURCE_DIR}/libs/vdf/CMakeLists.txt DESTINATION ${vdf_BINARY_DIR}/src)
  add_subdirectory(${vdf_BINARY_DIR}/src ${vdf_BINARY_DIR}/build EXCLUDE_FROM_ALL)
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_VDF
    FETCHCONTENT_UPDATES_DISCONNECTED_VDF)
  register_library_copyright("ValveFileVDF" "${vdf_SOURCE_DIR}/LICENSE")
endif ()
