# glad2
include_guard(GLOBAL)

add_subdirectory(${CMAKE_SOURCE_DIR}/cmake/deps/glad/ ${CMAKE_BINARY_DIR}/cmake/deps/glad/ EXCLUDE_FROM_ALL SYSTEM)
set_target_properties(glad PROPERTIES FOLDER libs)
register_copyright(glad "Glad 2" "${CMAKE_SOURCE_DIR}/cmake/deps/glad/LICENSE")
