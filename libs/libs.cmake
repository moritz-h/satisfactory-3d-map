# Dependencies

# Setup FetchContent
include(FetchContent)
mark_as_advanced(FORCE
  FETCHCONTENT_BASE_DIR
  FETCHCONTENT_FULLY_DISCONNECTED
  FETCHCONTENT_QUIET
  FETCHCONTENT_UPDATES_DISCONNECTED)

# ### LibSave Dependencies ###

# spdlog
FetchContent_Declare(spdlog
  URL "https://github.com/gabime/spdlog/archive/v1.13.0.tar.gz"
  URL_HASH SHA256=534f2ee1a4dcbeb22249856edfb2be76a1cf4f708a20b0ac2ed090ee24cfdbc9)
FetchContent_GetProperties(spdlog)
if (NOT spdlog_POPULATED)
  message(STATUS "Fetch spdlog ...")
  FetchContent_Populate(spdlog)
  option(SPDLOG_DISABLE_DEFAULT_LOGGER "" ON)
  option(SPDLOG_BUILD_PIC "" "${SATISFACTORY3DMAP_BUILD_PIC}")
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
    SPDLOG_SYSTEM_INCLUDES
    SPDLOG_TIDY
    SPDLOG_USE_STD_FORMAT
    SPDLOG_WCHAR_FILENAMES
    SPDLOG_WCHAR_SUPPORT)
  register_copyright(spdlog "spdlog" "${spdlog_SOURCE_DIR}/LICENSE")
endif ()

# zlib
FetchContent_Declare(zlib
  URL "https://github.com/madler/zlib/archive/v1.3.1.tar.gz"
  URL_HASH SHA256=17e88863f3600672ab49182f217281b6fc4d3c762bde361935e436a95214d05c)
FetchContent_GetProperties(zlib)
if (NOT zlib_POPULATED)
  message(STATUS "Fetch zlib ...")
  FetchContent_Populate(zlib)
  file(COPY ${CMAKE_SOURCE_DIR}/libs/zlib/CMakeLists.txt DESTINATION ${zlib_SOURCE_DIR})
  add_subdirectory(${zlib_SOURCE_DIR} ${zlib_BINARY_DIR} EXCLUDE_FROM_ALL)
  set_target_properties(zlibstatic PROPERTIES FOLDER libs)
  if (SATISFACTORY3DMAP_BUILD_PIC)
    set_target_properties(zlibstatic PROPERTIES POSITION_INDEPENDENT_CODE ON)
  endif ()
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_ZLIB
    FETCHCONTENT_UPDATES_DISCONNECTED_ZLIB)
  register_copyright(zlib "zlib" "${CMAKE_SOURCE_DIR}/libs/zlib/LICENSE.txt")
endif ()

# natsort
FetchContent_Declare(natsort
  URL "https://github.com/sourcefrog/natsort/archive/f8a6b0cda6de846b2a3b741967f35809de00b083.tar.gz"
  URL_HASH SHA256=3db24433dbd30d91c37385fe05b1e9af7bef9d233e500b656c20ee8bce0eb5a0)
FetchContent_GetProperties(natsort)
if (NOT natsort_POPULATED)
  message(STATUS "Fetch natsort ...")
  FetchContent_Populate(natsort)
  file(COPY ${CMAKE_SOURCE_DIR}/libs/natsort/CMakeLists.txt DESTINATION ${natsort_SOURCE_DIR})
  add_subdirectory(${natsort_SOURCE_DIR} ${natsort_BINARY_DIR} EXCLUDE_FROM_ALL)
  set_target_properties(natsort PROPERTIES FOLDER libs)
  if (SATISFACTORY3DMAP_BUILD_PIC)
    set_target_properties(natsort PROPERTIES POSITION_INDEPENDENT_CODE ON)
  endif ()
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_NATSORT
    FETCHCONTENT_UPDATES_DISCONNECTED_NATSORT)
  register_copyright(natsort "natsort" "${CMAKE_SOURCE_DIR}/libs/natsort/LICENSE.txt")
endif ()

# ### Map Dependencies ###

if (SATISFACTORY3DMAP_BUILD_APP)

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

  # glad2
  add_subdirectory(${CMAKE_SOURCE_DIR}/libs/glad/ EXCLUDE_FROM_ALL)
  set_target_properties(glad PROPERTIES FOLDER libs)
  register_copyright(glad "Glad 2" "${CMAKE_SOURCE_DIR}/libs/glad/LICENSE")

  # GLFW
  FetchContent_Declare(glfw
    URL "https://github.com/glfw/glfw/archive/3.3.9.tar.gz"
    URL_HASH SHA256=a7e7faef424fcb5f83d8faecf9d697a338da7f7a906fc1afbc0e1879ef31bd53)
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
    register_copyright(glfw "GLFW" "${glfw_SOURCE_DIR}/LICENSE.md")
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
    register_copyright(glm "GLM" "${glm_SOURCE_DIR}/copying.txt")
  endif ()

  # freetype
  FetchContent_Declare(freetype
    URL "https://github.com/freetype/freetype/archive/VER-2-13-2.tar.gz"
    URL_HASH SHA256=427201f5d5151670d05c1f5b45bef5dda1f2e7dd971ef54f0feaaa7ffd2ab90c)
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
    register_copyright(freetype "FreeType" "${freetype_SOURCE_DIR}/LICENSE.TXT")
  endif ()

  # imgui
  FetchContent_Declare(imgui
    URL "https://github.com/ocornut/imgui/archive/v1.90.3-docking.tar.gz"
    URL_HASH SHA256=ebd1da0f76a95a7a690f8a0dfa119e1c6da4eee40383e582fb75374792be0891)
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
    register_copyright(imgui "Dear ImGui" "${imgui_SOURCE_DIR}/LICENSE.txt")
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
    register_copyright(imguiclub "imgui_memory_editor" "${imguiclub_SOURCE_DIR}/LICENSE.txt")
  endif ()

  # json
  FetchContent_Declare(json
    URL "https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz"
    URL_HASH SHA256=d6c65aca6b1ed68e7a182f4757257b107ae403032760ed6ef121c9d55e81757d)
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
    register_copyright(json "JSON for Modern C++" "${json_SOURCE_DIR}/LICENSE.MIT")
  endif ()

  # tinygltf
  FetchContent_Declare(tinygltf
    URL "https://github.com/syoyo/tinygltf/archive/v2.8.21.tar.gz"
    URL_HASH SHA256=e567257d7addde58b0a483832cbaa5dd8f15e5bcaee6f023831e215d1a2c0502)
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
    register_copyright(tinygltf "TinyGLTF" "${tinygltf_SOURCE_DIR}/LICENSE")
  endif ()

  # glowl
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
    register_copyright(portable_file_dialogs "Portable File Dialogs" "${portable_file_dialogs_SOURCE_DIR}/COPYING")
  endif ()

  # iconfontcppheaders
  FetchContent_Declare(iconfontcppheaders
    URL "https://github.com/juliettef/IconFontCppHeaders/archive/8886c5657bac22b8fee34354871e3ade2a596433.tar.gz"
    URL_HASH SHA256=feb55951caf522cb1a660549f163b9ba94dafb3d1770f51c0bb532865db27000)
  FetchContent_GetProperties(iconfontcppheaders)
  if (NOT iconfontcppheaders_POPULATED)
    message(STATUS "Fetch iconfontcppheaders ...")
    FetchContent_Populate(iconfontcppheaders)
    file(COPY ${CMAKE_SOURCE_DIR}/libs/iconfontcppheaders/CMakeLists.txt DESTINATION ${iconfontcppheaders_SOURCE_DIR})
    add_subdirectory(${iconfontcppheaders_SOURCE_DIR} ${iconfontcppheaders_BINARY_DIR} EXCLUDE_FROM_ALL)
    mark_as_advanced(FORCE
      FETCHCONTENT_SOURCE_DIR_ICONFONTCPPHEADERS
      FETCHCONTENT_UPDATES_DISCONNECTED_ICONFONTCPPHEADERS)
    register_copyright(iconfontcppheaders "IconFontCppHeaders" "${iconfontcppheaders_SOURCE_DIR}/licence.txt")
  endif ()

  # vdf
  FetchContent_Declare(vdf
    URL "https://github.com/TinyTinni/ValveFileVDF/archive/c8adfc29e62cc980b595e965bedfb239087647ff.tar.gz"
    URL_HASH SHA256=48a078ff1e360d9c2217c9bd2eb766928a0234b73bb2bd458d04c64644669550)
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
    register_copyright(vdf "ValveFileVDF" "${vdf_SOURCE_DIR}/LICENSE")
  endif ()

endif ()

# ### Python Dependencies ###

if (SATISFACTORY3DMAP_BUILD_LIB_PYTHON)

  # pybind11
  FetchContent_Declare(pybind11
    URL "https://github.com/pybind/pybind11/archive/v2.11.1.tar.gz"
    URL_HASH SHA256=d475978da0cdc2d43b73f30910786759d593a9d8ee05b1b6846d1eb16c6d2e0c)
  FetchContent_GetProperties(pybind11)
  if (NOT pybind11_POPULATED)
    message(STATUS "Fetch pybind11 ...")
    FetchContent_Populate(pybind11)
    option(PYBIND11_INSTALL "" OFF)
    option(PYBIND11_TEST "" OFF)
    add_subdirectory(${pybind11_SOURCE_DIR} ${pybind11_BINARY_DIR} EXCLUDE_FROM_ALL)
    mark_as_advanced(FORCE
      FETCHCONTENT_SOURCE_DIR_PYBIND11
      FETCHCONTENT_UPDATES_DISCONNECTED_PYBIND11
      PYBIND11_FINDPYTHON
      PYBIND11_INSTALL
      PYBIND11_INTERNALS_VERSION
      PYBIND11_NOPYTHON
      PYBIND11_PYTHONLIBS_OVERWRITE
      PYBIND11_PYTHON_VERSION
      PYBIND11_SIMPLE_GIL_MANAGEMENT
      PYBIND11_TEST)
    register_copyright(pybind11 "pybind11" "${pybind11_SOURCE_DIR}/LICENSE")
  endif ()

endif ()
