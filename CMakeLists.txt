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
find_package(ZLIB REQUIRED)

# Targets
add_executable(${PROJECT_NAME})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME} PRIVATE ZLIB::ZLIB)
target_sources(${PROJECT_NAME} PRIVATE
  src/main.cpp
  src/SaveGame/SaveGame.cpp
  src/SaveGame/SaveGame.h
  src/SaveGame/Types/Properties/Property.h
  src/SaveGame/Types/ObjectReference.h
  src/SaveGame/Types/PropertyCollection.cpp
  src/SaveGame/Types/PropertyCollection.h
  src/SaveGame/Types/SaveActor.cpp
  src/SaveGame/Types/SaveActor.h
  src/SaveGame/Types/SaveObject.cpp
  src/SaveGame/Types/SaveObject.h
  src/SaveGame/Types/SaveObjectBase.cpp
  src/SaveGame/Types/SaveObjectBase.h
  src/SaveGame/Types/Vectors.h
  src/SaveGame/Utils/BufferUtils.h
  src/SaveGame/Utils/FileUtils.h)
