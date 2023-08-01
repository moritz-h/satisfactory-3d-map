# Copyright

include(file_utils)

set(copyright_dir "${CMAKE_BINARY_DIR}/copyright")
set(copyright_targets "" CACHE INTERNAL "" FORCE)

# Register copyright
function(register_copyright TARGET_NAME LIBRARY_NAME COPYRIGHT_FILE)
  # Validate
  if (NOT "${TARGET_NAME}" MATCHES "^[a-z0-9_]+$")
    message(FATAL_ERROR "Copyright target name must contain only lowercase letters, numbers, and underscores!")
  endif ()

  # Store list of targets
  list(APPEND copyright_targets "${TARGET_NAME}")
  set(copyright_targets "${copyright_targets}" CACHE INTERNAL "" FORCE)

  # Write content
  file(MAKE_DIRECTORY "${copyright_dir}/${TARGET_NAME}")
  file(READ "${COPYRIGHT_FILE}" content)
  write_file_if_changed("${copyright_dir}/${TARGET_NAME}/copyright" "${content}")
  write_file_if_changed("${copyright_dir}/${TARGET_NAME}/name" "${LIBRARY_NAME}")
endfunction()

# Remove unknown content from copyright dir
function(copyright_cleanup)
  file(GLOB dir_content RELATIVE "${copyright_dir}" "${copyright_dir}/*")
  foreach (entry ${dir_content})
    if (NOT "${entry}" IN_LIST copyright_targets)
      file(REMOVE_RECURSE "${copyright_dir}/${entry}")
    endif ()
  endforeach ()
endfunction()
