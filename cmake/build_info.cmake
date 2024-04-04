# Build info

include(file_utils)

# Directories
set(build_info_dir "${CMAKE_BINARY_DIR}/build_info")

# Static info
write_file_if_changed("${build_info_dir}/VERSION_FULL" "${PROJECT_VERSION}")

set(build_info_script_generated_files
  ${build_info_dir}/GIT_VERSION
  ${build_info_dir}/LICENSE)

add_custom_target(build_info_script
  BYPRODUCTS ${build_info_script_generated_files}
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/cmake/build_info_script.cmake)
set_target_properties(build_info_script PROPERTIES FOLDER libs)

set(build_info_files
  ${build_info_dir}/VERSION_FULL
  ${build_info_script_generated_files})
