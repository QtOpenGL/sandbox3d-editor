cmake_minimum_required(VERSION 3.1)

INCLUDE(CheckIncludeFiles)

file(GLOB_RECURSE UI_FILES ../../resources/ui/*.ui)
file(GLOB_RECURSE SRC_FILES *.cpp *.h *.inl ../include/*.h ../include/*.inl)

qt5_wrap_ui(UI_HEADER_FILES ${UI_FILES})

# Compile application
add_executable(${PROJECT_NAME} ${SRC_FILES} ${UI_FILES})

IF(CMAKE_BUILD_TYPE MATCHES Debug)
        target_link_libraries(${PROJECT_NAME} PUBLIC glBreak)
ENDIF()

target_link_libraries(${PROJECT_NAME} PRIVATE sandbox_resources scene Qt5::Core Qt5::Widgets Qt5::Gui Qt5::OpenGL Qt5::Xml GraphWidget Graph)
