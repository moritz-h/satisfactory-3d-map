cmake_minimum_required(VERSION 3.21...3.24 FATAL_ERROR)

# Functions
function(write_file_if_changed filename content)
  if (EXISTS "${filename}")
    file(READ ${filename} content_old)
    if ("${content}" STREQUAL "${content_old}")
      # File exists already with same content. Do not write, to not trigger unnecessary rebuild.
      return()
    endif ()
  endif ()
  file(WRITE ${filename} "${content}")
endfunction()

# Directories
get_filename_component(INFO_SRC_DIR ${CMAKE_SCRIPT_MODE_FILE} DIRECTORY)
get_filename_component(PROJECT_DIR ${INFO_SRC_DIR} DIRECTORY)
set(INFO_RESOURCES_DIR "${CMAKE_BINARY_DIR}/resources/build_info")

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
