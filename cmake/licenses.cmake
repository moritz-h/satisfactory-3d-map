# Licenses

set(licenses_dir "${CMAKE_BINARY_DIR}/licenses")
set(licenses_list "${licenses_dir}/licenses.list")
file(WRITE "${licenses_list}" "")
set(licenses_files "${licenses_list}" CACHE INTERNAL "" FORCE)
set(licenses_count 0 CACHE INTERNAL "" FORCE)

function(register_library_license LIBRARY_NAME LICENSE_FILE)
  file(APPEND "${licenses_list}" "${LIBRARY_NAME}\n")
  set(filename "${licenses_dir}/library_${licenses_count}.txt")
  file(COPY_FILE "${LICENSE_FILE}" "${filename}")

  list(APPEND licenses_files "${filename}")
  set(licenses_files "${licenses_files}" CACHE INTERNAL "" FORCE)
  math(EXPR count "${licenses_count} + 1")
  set(licenses_count "${count}" CACHE INTERNAL "" FORCE)
endfunction()
