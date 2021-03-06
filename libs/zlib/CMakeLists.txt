cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

cmake_policy(SET CMP0091 NEW)

project(zlib
  VERSION 1.2.11
  LANGUAGES C)

set(ZLIB_PUBLIC_HDRS
  zconf.h
  zlib.h
)
set(ZLIB_PRIVATE_HDRS
  crc32.h
  deflate.h
  gzguts.h
  inffast.h
  inffixed.h
  inflate.h
  inftrees.h
  trees.h
  zutil.h
)
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
  zutil.c
)

add_library(zlibstatic STATIC ${ZLIB_SRCS} ${ZLIB_PUBLIC_HDRS} ${ZLIB_PRIVATE_HDRS})

target_include_directories(zlibstatic PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
set_target_properties(zlibstatic PROPERTIES
  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
