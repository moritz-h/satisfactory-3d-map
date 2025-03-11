cmake_minimum_required(VERSION 3.28...3.31 FATAL_ERROR)

project(zlib
  VERSION 1.3.1
  LANGUAGES C)

set(ZLIB_PUBLIC_HDRS
  zconf.h
  zlib.h)
set(ZLIB_PRIVATE_HDRS
  crc32.h
  deflate.h
  gzguts.h
  inffast.h
  inffixed.h
  inflate.h
  inftrees.h
  trees.h
  zutil.h)
set(ZLIB_SRCS
  adler32.c
  compress.c
  crc32.c
  deflate.c
  gzclose.c
  gzlib.c
  gzread.c
  gzwrite.c
  inflate.c
  infback.c
  inftrees.c
  inffast.c
  trees.c
  uncompr.c
  zutil.c)

include(CheckIncludeFile)
check_include_file(unistd.h Z_HAVE_UNISTD_H)
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/zconf.h")
  file(RENAME "${CMAKE_CURRENT_SOURCE_DIR}/zconf.h" "${CMAKE_CURRENT_SOURCE_DIR}/zconf.h.included")
endif ()
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/zconf.h.cmakein" "${CMAKE_CURRENT_BINARY_DIR}/zconf.h" @ONLY)

add_library(zlibstatic STATIC ${ZLIB_SRCS} ${ZLIB_PUBLIC_HDRS} ${ZLIB_PRIVATE_HDRS})
add_library(ZLIB::ZLIBSTATIC ALIAS zlibstatic)

target_include_directories(zlibstatic PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)
set_target_properties(zlibstatic PROPERTIES
  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
