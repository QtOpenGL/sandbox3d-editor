cmake_minimum_required(VERSION 3.1)

find_package(Qt5 COMPONENTS Core REQUIRED)

if(Qt5_POSITION_INDEPENDENT_CODE)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

file(GLOB_RECURSE RC_FILES *.qrc)
file(GLOB_RECURSE SHADERS_FILES shaders/*.vert shaders/*.geom shaders/*.frag)

qt5_add_resources(RES_SOURCES ${RC_FILES})

add_library(sandbox_resources ${RES_SOURCES} ${SHADERS_FILES})
set_target_properties(sandbox_resources PROPERTIES LINKER_LANGUAGE CXX)
