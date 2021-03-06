cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

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

# Targets
add_executable(${PROJECT_NAME})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME} PRIVATE zlibstatic glad glfw imgui)
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
