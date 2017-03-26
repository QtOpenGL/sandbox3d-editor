cmake_minimum_required(VERSION 2.8.12)

INCLUDE(CheckIncludeFiles)

file(GLOB_RECURSE RC_FILES *.qrc)
file(GLOB_RECURSE SHADERS_FILES shaders/*.vert shaders/*.geom shaders/*.frag)

find_package(Qt5 COMPONENTS Core)

if(Qt5_POSITION_INDEPENDENT_CODE)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

qt5_add_resources(RES_SOURCES ${RC_FILES})

# Compile application
add_library(sandbox_resources ${RES_SOURCES} ${SHADERS_FILES})
set_target_properties(sandbox_resources PROPERTIES LINKER_LANGUAGE CXX)
