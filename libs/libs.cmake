# Dependencies

# ### LibSave Dependencies ###

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
  URL "https://github.com/sourcefrog/natsort/archive/cdd8df9602e727482ae5e051cff74b7ec7ffa07a.tar.gz"
  URL_HASH SHA256=5b24abd4011ffe4754839a4f2648ed6dd64c8b28c95836bcecc5c7dc868108af)
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
    URL "https://github.com/glfw/glfw/archive/3.4.tar.gz"
    URL_HASH SHA256=c038d34200234d071fae9345bc455e4a8f2f544ab60150765d7704e08f3dac01)
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
      GLFW_BUILD_WIN32
      GLFW_BUILD_X11
      GLFW_BUILD_WAYLAND
      GLFW_BUILD_COCOA
      GLFW_INSTALL
      GLFW_LIBRARY_TYPE
      GLFW_USE_HYBRID_HPG
      USE_MSVC_RUNTIME_LIBRARY_DLL
      X11_xcb_icccm_INCLUDE_PATH
      X11_xcb_icccm_LIB
      X11_xcb_xkb_INCLUDE_PATH)
    register_copyright(glfw "GLFW" "${glfw_SOURCE_DIR}/LICENSE.md")
  endif ()

  # glm
  FetchContent_Declare(glm
    URL "https://github.com/g-truc/glm/archive/1.0.1.tar.gz"
    URL_HASH SHA256=9f3174561fd26904b23f0db5e560971cbf9b3cbda0b280f04d5c379d03bf234c)
  FetchContent_GetProperties(glm)
  if (NOT glm_POPULATED)
    message(STATUS "Fetch glm ...")
    FetchContent_Populate(glm)
    add_subdirectory(${glm_SOURCE_DIR} ${glm_BINARY_DIR} EXCLUDE_FROM_ALL)
    target_compile_definitions(glm PUBLIC GLM_ENABLE_EXPERIMENTAL)
    set_target_properties(glm PROPERTIES FOLDER libs)
    mark_as_advanced(FORCE
      FETCHCONTENT_SOURCE_DIR_GLM
      FETCHCONTENT_UPDATES_DISCONNECTED_GLM
      GLM_BUILD_INSTALL
      GLM_BUILD_LIBRARY
      GLM_BUILD_TESTS
      GLM_DISABLE_AUTO_DETECTION
      GLM_ENABLE_CXX_11
      GLM_ENABLE_CXX_14
      GLM_ENABLE_CXX_17
      GLM_ENABLE_CXX_20
      GLM_ENABLE_CXX_98
      GLM_ENABLE_FAST_MATH
      GLM_ENABLE_LANG_EXTENSIONS
      GLM_ENABLE_SIMD_AVX
      GLM_ENABLE_SIMD_AVX2
      GLM_ENABLE_SIMD_SSE2
      GLM_ENABLE_SIMD_SSE3
      GLM_ENABLE_SIMD_SSE4_1
      GLM_ENABLE_SIMD_SSE4_2
      GLM_ENABLE_SIMD_SSSE3
      GLM_FORCE_PURE)
    register_copyright(glm "GLM" "${glm_SOURCE_DIR}/copying.txt")
  endif ()

  # freetype
  FetchContent_Declare(freetype
    URL "https://github.com/freetype/freetype/archive/VER-2-13-3.tar.gz"
    URL_HASH SHA256=bc5c898e4756d373e0d991bab053036c5eb2aa7c0d5c67e8662ddc6da40c4103)
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
    URL "https://github.com/ocornut/imgui/archive/v1.91.8-docking.tar.gz"
    URL_HASH SHA256=55f5e65abea635f2a8bfa9a92cd966448a363a262cf6dead7cc662fb0ab37612)
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
    URL "https://github.com/ocornut/imgui_club/archive/17510b03602c8b6c7ed48fd8fa29d9a75eb10dc6.tar.gz"
    URL_HASH SHA256=21ef4e6ea2a34b03b54925df96dbbd1ee16e20e0be7ef981085fdb23715aa0b6)
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
    URL "https://github.com/syoyo/tinygltf/archive/v2.9.5.tar.gz"
    URL_HASH SHA256=7b93da27c524dd17179a0eeba6f432b0060d82f6222630ba027c219ce11e24db)
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
    file(COPY ${CMAKE_SOURCE_DIR}/libs/portable_file_dialogs/CMakeLists.txt DESTINATION ${portable_file_dialogs_SOURCE_DIR})
    add_subdirectory(${portable_file_dialogs_SOURCE_DIR} ${portable_file_dialogs_BINARY_DIR} EXCLUDE_FROM_ALL)
    mark_as_advanced(FORCE
      FETCHCONTENT_SOURCE_DIR_PORTABLE_FILE_DIALOGS
      FETCHCONTENT_UPDATES_DISCONNECTED_PORTABLE_FILE_DIALOGS)
    register_copyright(portable_file_dialogs "Portable File Dialogs" "${portable_file_dialogs_SOURCE_DIR}/COPYING")
  endif ()

  # iconfontcppheaders
  FetchContent_Declare(iconfontcppheaders
    URL "https://github.com/juliettef/IconFontCppHeaders/archive/ef464d2fe5a568d30d7c88138e78d7fac7cfebc5.tar.gz"
    URL_HASH SHA256=a3535d255d930f646a98f01eaea456d00324e46d6577c119ce01fcc1f4f39af7)
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
    URL "https://github.com/TinyTinni/ValveFileVDF/archive/97a021ce2b7ab3e950f6b0a4f373ef7becbafe15.tar.gz"
    URL_HASH SHA256=04001c0ad97f9ccbcfd416c8b3ceb0f71604b1af894db89031a490192e4d8ee2)
  FetchContent_GetProperties(vdf)
  if (NOT vdf_POPULATED)
    message(STATUS "Fetch vdf ...")
    FetchContent_Populate(vdf)
    add_subdirectory(${vdf_SOURCE_DIR} ${vdf_BINARY_DIR} EXCLUDE_FROM_ALL)
    set_target_properties(ValveFileVDF PROPERTIES FOLDER libs)
    mark_as_advanced(FORCE
      FETCHCONTENT_SOURCE_DIR_VDF
      FETCHCONTENT_UPDATES_DISCONNECTED_VDF
      VALVEFILEVDF_ENABLE_FUZZING
      VALVEFILEVDF_ENABLE_PYTHON
      VALVEFILEVDF_ENABLE_TESTING)
    register_copyright(vdf "ValveFileVDF" "${vdf_SOURCE_DIR}/LICENSE")
  endif ()

endif ()

# ### Python Dependencies ###

if (SATISFACTORY3DMAP_BUILD_LIB_PYTHON)

  # pybind11
  FetchContent_Declare(pybind11
    URL "https://github.com/pybind/pybind11/archive/v2.13.6.tar.gz"
    URL_HASH SHA256=e08cb87f4773da97fa7b5f035de8763abc656d87d5773e62f6da0587d1f0ec20)
  FetchContent_GetProperties(pybind11)
  if (NOT pybind11_POPULATED)
    message(STATUS "Fetch pybind11 ...")
    FetchContent_Populate(pybind11)
    option(PYBIND11_FINDPYTHON "" ON)
    option(PYBIND11_INSTALL "" OFF)
    option(PYBIND11_TEST "" OFF)
    add_subdirectory(${pybind11_SOURCE_DIR} ${pybind11_BINARY_DIR} EXCLUDE_FROM_ALL)
    mark_as_advanced(FORCE
      FETCHCONTENT_SOURCE_DIR_PYBIND11
      FETCHCONTENT_UPDATES_DISCONNECTED_PYBIND11
      PYBIND11_DISABLE_HANDLE_TYPE_NAME_DEFAULT_IMPLEMENTATION
      PYBIND11_FINDPYTHON
      PYBIND11_INSTALL
      PYBIND11_INTERNALS_VERSION
      PYBIND11_NOPYTHON
      PYBIND11_NUMPY_1_ONLY
      PYBIND11_PYTHONLIBS_OVERWRITE
      PYBIND11_PYTHON_VERSION
      PYBIND11_SIMPLE_GIL_MANAGEMENT
      PYBIND11_TEST
      PYBIND11_USE_CROSSCOMPILING)
    register_copyright(pybind11 "pybind11" "${pybind11_SOURCE_DIR}/LICENSE")
  endif ()

endif ()
