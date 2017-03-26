cmake_minimum_required(VERSION 2.8.12)

INCLUDE(CheckIncludeFiles)

file(GLOB_RECURSE UI_FILES ../ui/*.ui)
file(GLOB_RECURSE SRC_FILES *.cpp *.h *.inl ../include/*.h ../include/*.inl)

# Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Gui)
find_package(Qt5OpenGL)

include_directories(
	${Qt5Core_INCLUDE_DIRS}
	${Qt5Widgets_INCLUDE_DIRS}
	${Qt5Gui_INCLUDE_DIRS}
	${Qt5OpenGL_INCLUDE_DIRS}
)

if(Qt5_POSITION_INDEPENDENT_CODE)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

qt5_wrap_ui(UI_HEADER_FILES ${UI_FILES})

# Assimp
find_package(assimp REQUIRED)

# Jansson
find_library(JANSSON_LIBRARY jansson)

# Compile application
add_executable(${PROJECT_NAME} ${SRC_FILES} ${UI_FILES})

target_link_libraries(${PROJECT_NAME} sandbox_resources Sandbox3D ${assimp_LIBRARIES} ${JANSSON_LIBRARY} Qt5::Core Qt5::Widgets Qt5::Gui Qt5::OpenGL Qt5NodeEditor)
