# tinygltf
include_guard(GLOBAL)

find_package(nlohmann_json REQUIRED)

FetchContent_Declare(tinygltf
  URL "https://github.com/syoyo/tinygltf/archive/v2.9.5.tar.gz"
  URL_HASH SHA256=7b93da27c524dd17179a0eeba6f432b0060d82f6222630ba027c219ce11e24db
  SOURCE_SUBDIR non-existing-path-to-prevent-add-subdirectory
  EXCLUDE_FROM_ALL
  SYSTEM)
message(STATUS "Fetch tinygltf ...")
FetchContent_MakeAvailable(tinygltf)
# Build a custom tinygltf version without using the internal json.hpp. Link our json target instead.
# Further, the current tinygltf CMake does not work well with add_subdirectory and building as static library.
file(COPY ${tinygltf_SOURCE_DIR}/stb_image.h DESTINATION ${tinygltf_BINARY_DIR}/src/include)
file(COPY ${tinygltf_SOURCE_DIR}/stb_image_write.h DESTINATION ${tinygltf_BINARY_DIR}/src/include)
file(COPY ${tinygltf_SOURCE_DIR}/tiny_gltf.h DESTINATION ${tinygltf_BINARY_DIR}/src/include)
file(COPY ${tinygltf_SOURCE_DIR}/tiny_gltf.cc DESTINATION ${tinygltf_BINARY_DIR}/src/src)
file(COPY ${CMAKE_SOURCE_DIR}/cmake/deps/tinygltf/CMakeLists.txt DESTINATION ${tinygltf_BINARY_DIR}/src)
add_subdirectory(${tinygltf_BINARY_DIR}/src ${tinygltf_BINARY_DIR}/build EXCLUDE_FROM_ALL SYSTEM)
set_target_properties(tinygltf PROPERTIES FOLDER libs)
mark_as_advanced(FORCE
  FETCHCONTENT_SOURCE_DIR_TINYGLTF
  FETCHCONTENT_UPDATES_DISCONNECTED_TINYGLTF)
register_copyright(tinygltf "TinyGLTF" "${tinygltf_SOURCE_DIR}/LICENSE")
