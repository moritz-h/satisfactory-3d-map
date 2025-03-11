cmake_minimum_required(VERSION 3.28...3.31 FATAL_ERROR)

project(portable_file_dialogs VERSION 1.0 LANGUAGES CXX)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
