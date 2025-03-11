cmake_minimum_required(VERSION 3.28...3.31 FATAL_ERROR)

project(iconfontcppheaders)

add_library(iconfontcppheaders INTERFACE)
target_include_directories(iconfontcppheaders INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
