# Warning level

function(set_warning_level target)
  if (SATISFACTORY3DMAP_STRICT_WARNINGS)
    if (MSVC)
      target_compile_options(${target} PRIVATE /W4 /WX)
      target_compile_definitions(${target} PRIVATE
        _CRT_SECURE_NO_WARNINGS
        _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING)
    else ()
      target_compile_options(${target} PRIVATE -Werror -Wall -Wextra -pedantic -pedantic-errors)
    endif ()
  endif ()

endfunction()
