cmake_minimum_required(VERSION 2.8.12)

INCLUDE(CheckIncludeFiles)

file(GLOB_RECURSE UI_FILES ../../resources/ui/*.ui)
file(GLOB_RECURSE SRC_FILES *.cpp *.h *.inl ../include/*.h ../include/*.inl)

# Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Gui)
find_package(Qt5OpenGL)

if(Qt5_POSITION_INDEPENDENT_CODE)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

qt5_wrap_ui(UI_HEADER_FILES ${UI_FILES})

set(RENDERER_LIB "" CACHE FILEPATH "Path to the renderer library")
add_definitions(-DRENDERER_LIB=\"${RENDERER_LIB}\")

# Compile application
add_executable(${PROJECT_NAME} ${SRC_FILES} ${UI_FILES})

IF(CMAKE_BUILD_TYPE MATCHES Debug)
        target_link_libraries(${PROJECT_NAME} PUBLIC glBreak)
ENDIF()

target_link_libraries(${PROJECT_NAME} PRIVATE sandbox_resources scene Qt5::Core Qt5::Widgets Qt5::Gui Qt5::OpenGL GraphWidget Graph)

# Jansson
find_library(JANSSON_LIBRARY jansson)
target_link_libraries(${PROJECT_NAME} PRIVATE ${JANSSON_LIBRARY})
