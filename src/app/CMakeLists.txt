cmake_minimum_required(VERSION 3.1)

file(GLOB_RECURSE UI_FILES ../../resources/ui/*.ui)
file(GLOB_RECURSE SRC_FILES *.cpp *.h *.inl ../include/*.h ../include/*.inl)

qt5_wrap_ui(UI_HEADER_FILES ${UI_FILES})

add_executable(Sandbox3D-Editor ${SRC_FILES} ${UI_FILES})

target_link_libraries(Sandbox3D-Editor PRIVATE sandbox_resources GraphWidget Qt5::Core Qt5::Widgets Qt5::Gui Qt5::OpenGL Qt5::Xml)
target_link_libraries(Sandbox3D-Editor PRIVATE scene)
target_link_libraries(Sandbox3D-Editor PRIVATE RenderGraph Graph)
target_include_directories(Sandbox3D-Editor PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../")

if (CMAKE_BUILD_TYPE MATCHES Debug AND NOT WIN32)
        target_link_libraries(Sandbox3D-Editor PUBLIC glBreak)
endif(CMAKE_BUILD_TYPE MATCHES Debug AND NOT WIN32)

if (WIN32)
	find_program(WINDEPLOYQT_EXECUTABLE NAMES windeployqt HINTS C:\\Qt\\5.10.1\\msvc2017_64\\bin) # default Qt 5.10.1 (VS2017) install path / TODO : detect this path automatically
    add_custom_command(TARGET Sandbox3D-Editor POST_BUILD COMMAND ${WINDEPLOYQT_EXECUTABLE} $<TARGET_FILE:Sandbox3D-Editor>)
endif(WIN32)
