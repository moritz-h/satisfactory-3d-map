# Copyright

set(copyright_dir "${CMAKE_BINARY_DIR}/copyright")
set(copyright_library_list "${copyright_dir}/library.list")
set(copyright_resource_list "${copyright_dir}/resource.list")
file(WRITE "${copyright_library_list}" "")
file(WRITE "${copyright_resource_list}" "")
set(copyright_files "${copyright_library_list};${copyright_resource_list}" CACHE INTERNAL "" FORCE)
set(copyright_library_count 0 CACHE INTERNAL "" FORCE)
set(copyright_resource_count 0 CACHE INTERNAL "" FORCE)

function(register_library_copyright LIBRARY_NAME COPYRIGHT_FILE)
  file(APPEND "${copyright_library_list}" "${LIBRARY_NAME}\n")
  set(filename "${copyright_dir}/library_${copyright_library_count}.txt")
  file(COPY_FILE "${COPYRIGHT_FILE}" "${filename}")

  list(APPEND copyright_files "${filename}")
  set(copyright_files "${copyright_files}" CACHE INTERNAL "" FORCE)
  math(EXPR count "${copyright_library_count} + 1")
  set(copyright_library_count "${count}" CACHE INTERNAL "" FORCE)
endfunction()

function(register_resource_copyright RESOURCE_NAME COPYRIGHT_FILE)
  file(APPEND "${copyright_resource_list}" "${RESOURCE_NAME}\n")
  set(filename "${copyright_dir}/resource_${copyright_resource_count}.txt")
  file(COPY_FILE "${COPYRIGHT_FILE}" "${filename}")

  list(APPEND copyright_files "${filename}")
  set(copyright_files "${copyright_files}" CACHE INTERNAL "" FORCE)
  math(EXPR count "${copyright_resource_count} + 1")
  set(copyright_resource_count "${count}" CACHE INTERNAL "" FORCE)
endfunction()
