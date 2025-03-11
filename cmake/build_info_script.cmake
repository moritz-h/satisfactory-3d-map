cmake_minimum_required(VERSION 3.28...3.31 FATAL_ERROR)

# Directories
get_filename_component(cmake_modules_dir ${CMAKE_SCRIPT_MODE_FILE} DIRECTORY)
get_filename_component(project_dir ${cmake_modules_dir} DIRECTORY)
set(build_info_dir "${CMAKE_BINARY_DIR}/build_info")

# Utils
include("${cmake_modules_dir}/file_utils.cmake")

# Git version
find_package(Git)
if (Git_FOUND AND IS_DIRECTORY "${project_dir}/.git")
  execute_process(COMMAND
    ${GIT_EXECUTABLE} describe --always --abbrev=12 --dirty
    WORKING_DIRECTORY "${project_dir}"
    OUTPUT_VARIABLE GIT_VERSION
    ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
else ()
  set(GIT_VERSION "no-git-info-available")
endif ()
write_file_if_changed(${build_info_dir}/GIT_VERSION "${GIT_VERSION}")

# License
configure_file(${project_dir}/LICENSE ${build_info_dir}/LICENSE COPYONLY)
