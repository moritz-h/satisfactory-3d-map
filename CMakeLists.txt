cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

# Policies
cmake_policy(SET CMP0091 NEW)

# Disable in source build
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# Project
project(SatisfactorySaveGame
  VERSION 0.1.0
  LANGUAGES CXX)

# Set a default build type if none was specified
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif ()

# Dependencies
include(libs/libs.cmake)

# Disable extra console window on Windows.
set(WIN32_FLAG "")
if (MSVC)
  option(SATISFACTORYSAVEGAME_DISABLE_CONSOLE "Disable extra console window." OFF)
  if (SATISFACTORYSAVEGAME_DISABLE_CONSOLE)
    set(WIN32_FLAG "WIN32")
  endif ()
endif (MSVC)

# Targets
add_executable(${PROJECT_NAME} ${WIN32_FLAG})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_EXTENSIONS OFF
  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME} PRIVATE zlibstatic glad glfw imgui)
if (MSVC AND SATISFACTORYSAVEGAME_DISABLE_CONSOLE)
  target_link_options(${PROJECT_NAME} PRIVATE "/entry:mainCRTStartup")
endif ()
target_sources(${PROJECT_NAME} PRIVATE
  src/main.cpp
  src/SaveGame/Objects/SaveActor.cpp
  src/SaveGame/Objects/SaveActor.h
  src/SaveGame/Objects/SaveObject.cpp
  src/SaveGame/Objects/SaveObject.h
  src/SaveGame/Objects/SaveObjectBase.cpp
  src/SaveGame/Objects/SaveObjectBase.h
  src/SaveGame/SaveGame.cpp
  src/SaveGame/SaveGame.h
  src/SaveGame/Types/ChunkHeader.cpp
  src/SaveGame/Types/ChunkHeader.h
  src/SaveGame/Types/ObjectReference.h
  src/SaveGame/Types/Properties/ArrayProperty.cpp
  src/SaveGame/Types/Properties/ArrayProperty.h
  src/SaveGame/Types/Properties/BoolProperty.cpp
  src/SaveGame/Types/Properties/BoolProperty.h
  src/SaveGame/Types/Properties/ByteProperty.cpp
  src/SaveGame/Types/Properties/ByteProperty.h
  src/SaveGame/Types/Properties/EnumProperty.cpp
  src/SaveGame/Types/Properties/EnumProperty.h
  src/SaveGame/Types/Properties/FloatProperty.cpp
  src/SaveGame/Types/Properties/FloatProperty.h
  src/SaveGame/Types/Properties/Int64Property.cpp
  src/SaveGame/Types/Properties/Int64Property.h
  src/SaveGame/Types/Properties/IntProperty.cpp
  src/SaveGame/Types/Properties/IntProperty.h
  src/SaveGame/Types/Properties/MapProperty.cpp
  src/SaveGame/Types/Properties/MapProperty.h
  src/SaveGame/Types/Properties/NameProperty.cpp
  src/SaveGame/Types/Properties/NameProperty.h
  src/SaveGame/Types/Properties/ObjectProperty.cpp
  src/SaveGame/Types/Properties/ObjectProperty.h
  src/SaveGame/Types/Properties/Property.cpp
  src/SaveGame/Types/Properties/Property.h
  src/SaveGame/Types/Properties/StrProperty.cpp
  src/SaveGame/Types/Properties/StrProperty.h
  src/SaveGame/Types/Properties/StructProperty.cpp
  src/SaveGame/Types/Properties/StructProperty.h
  src/SaveGame/Types/Properties/TextProperty.cpp
  src/SaveGame/Types/Properties/TextProperty.h
  src/SaveGame/Types/SaveHeader.cpp
  src/SaveGame/Types/SaveHeader.h
  src/SaveGame/Types/PropertyCollection.cpp
  src/SaveGame/Types/PropertyCollection.h
  src/SaveGame/Types/Vectors.h
  src/SaveGame/Utils/StreamUtils.h
  src/SaveGame/Utils/ZlibUtils.cpp
  src/SaveGame/Utils/ZlibUtils.h
  src/SaveViewer/MainWindow.cpp
  src/SaveViewer/MainWindow.h)

# Install
include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# Package
set(SATISFACTORYSAVEGAME_CPACK_GENERATOR "ZIP" CACHE STRING "Choose CPack generator.")
if (UNIX)
  set_property(CACHE SATISFACTORYSAVEGAME_CPACK_GENERATOR PROPERTY STRINGS "TGZ" "ZIP" "STGZ" "DEB")
else ()
  set_property(CACHE SATISFACTORYSAVEGAME_CPACK_GENERATOR PROPERTY STRINGS "ZIP" "NSIS")
endif ()

set(CPACK_GENERATOR "${SATISFACTORYSAVEGAME_CPACK_GENERATOR}")
set(CPACK_PACKAGE_VENDOR "SatisfactorySaveGame")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "SatisfactorySaveGame.")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

if (SATISFACTORYSAVEGAME_CPACK_GENERATOR STREQUAL "DEB")
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER "SatisfactorySaveGame")
  set(CPACK_DEBIAN_FILE_NAME "DEB-DEFAULT")
  set(CPACK_DEBIAN_PACKAGE_DEPENDS "")
  # https://stackoverflow.com/a/7363073
  set(CPACK_SET_DESTDIR true)
  set(CPACK_INSTALL_PREFIX /opt/satisfactorysavegame)
elseif (SATISFACTORYSAVEGAME_CPACK_GENERATOR STREQUAL "NSIS")
  # Start menu entry
  set(CPACK_PACKAGE_EXECUTABLES "SatisfactorySaveGame;SatisfactorySaveGame")
endif ()

# TODO: configure source packages

include(CPack)

# Enable strict warnings
option(SATISFACTORYSAVEGAME_STRICT_WARNINGS "Enable strict compiler warnings." OFF)
if (SATISFACTORYSAVEGAME_STRICT_WARNINGS)
  if (MSVC)
    # TODO: For now do not set "/WX" flag, as external libs generate warnings, which we cannot fix.
    # MSVC has an experimental feature to disable external warnings, see https://devblogs.microsoft.com/cppblog/broken-warnings-theory/.
    # Maybe in future there is cmake support for this feature, see https://gitlab.kitware.com/cmake/cmake/-/issues/17904.
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
  else ()
    target_compile_options(${PROJECT_NAME} PRIVATE -Werror -Wall -Wextra -pedantic -pedantic-errors)
  endif ()
endif ()

# Visual Studio folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
if (MSVC)
  set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif ()
