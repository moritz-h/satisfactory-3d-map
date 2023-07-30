cmake_minimum_required(VERSION 3.21...3.26 FATAL_ERROR)

# Directories
get_filename_component(INFO_SRC_DIR ${CMAKE_SCRIPT_MODE_FILE} DIRECTORY)
get_filename_component(PROJECT_DIR ${INFO_SRC_DIR} DIRECTORY)
set(INFO_RESOURCES_DIR "${CMAKE_BINARY_DIR}/resources/build_info")

# Utils
include("${INFO_SRC_DIR}/file_utils.cmake")

# Git version
find_package(Git)
if (Git_FOUND AND IS_DIRECTORY "${PROJECT_DIR}/.git")
  execute_process(COMMAND
    ${GIT_EXECUTABLE} describe --always --abbrev=12 --dirty
    WORKING_DIRECTORY "${PROJECT_DIR}"
    OUTPUT_VARIABLE GIT_VERSION
    ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
else ()
  set(GIT_VERSION "no-git-info-available")
endif ()
write_file_if_changed(${INFO_RESOURCES_DIR}/GIT_VERSION "${GIT_VERSION}")

# License
configure_file(${PROJECT_DIR}/LICENSE ${INFO_RESOURCES_DIR}/LICENSE COPYONLY)
