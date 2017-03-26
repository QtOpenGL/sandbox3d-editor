cmake_minimum_required(VERSION 2.8.12)

INCLUDE(CheckIncludeFiles)

file(GLOB_RECURSE RC_FILES *.qrc)
file(GLOB_RECURSE UI_FILES ui/*.ui)
file(GLOB_RECURSE SHADERS_FILES shaders/*.vert shaders/*.geom shaders/*.frag)

# Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if(Qt5_POSITION_INDEPENDENT_CODE)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

qt5_wrap_ui(UI_HEADER_FILES ${UI_FILES})

# Compile application
add_library(sandbox_resources ${UI_FILES} ${RC_FILES} ${SHADERS_FILES})
set_target_properties(sandbox_resources PROPERTIES LINKER_LANGUAGE CXX)

