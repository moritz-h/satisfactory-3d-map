# Build info

include(file_utils)

# Directories
set(INFO_RESOURCES_DIR "${CMAKE_BINARY_DIR}/resources/build_info")

# Static info
write_file_if_changed("${INFO_RESOURCES_DIR}/VERSION_FULL" "${PROJECT_VERSION}")

set(BUILD_INFO_SCRIPT_GENERATED_FILES
  ${INFO_RESOURCES_DIR}/GIT_VERSION
  ${INFO_RESOURCES_DIR}/LICENSE)

add_custom_target(build_info_script
  BYPRODUCTS ${BUILD_INFO_SCRIPT_GENERATED_FILES}
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/cmake/build_info_script.cmake)

set(build_info_files
  ${INFO_RESOURCES_DIR}/VERSION_FULL
  ${BUILD_INFO_SCRIPT_GENERATED_FILES})
