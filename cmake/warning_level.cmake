# Warning level

function(set_warning_level target)
  if (SATISFACTORY3DMAP_STRICT_WARNINGS)
    if (MSVC)
      # MSVC has an experimental feature to disable warnings from external libs, see https://devblogs.microsoft.com/cppblog/broken-warnings-theory/.
      # Maybe in future there is CMake support for this feature, see https://gitlab.kitware.com/cmake/cmake/-/issues/17904.
      target_compile_options(${target} PRIVATE /W4 /WX /external:anglebrackets /external:W3)
      target_compile_definitions(${target} PRIVATE
        _CRT_SECURE_NO_WARNINGS
        _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING)
    else ()
      target_compile_options(${target} PRIVATE -Werror -Wall -Wextra -pedantic -pedantic-errors)
    endif ()
  endif ()

endfunction()
