# File utils

function(write_file_if_changed filename content)
  if (EXISTS "${filename}")
    file(READ "${filename}" content_old)
    if ("${content}" STREQUAL "${content_old}")
      # File exists already with same content. Do not write, to not trigger unnecessary rebuild.
      return()
    endif ()
  endif ()
  file(WRITE "${filename}" "${content}")
endfunction()
