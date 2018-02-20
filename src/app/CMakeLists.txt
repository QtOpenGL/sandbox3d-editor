cmake_minimum_required(VERSION 3.1)

file(GLOB_RECURSE UI_FILES ../../resources/ui/*.ui)
file(GLOB_RECURSE SRC_FILES *.cpp *.h *.inl ../include/*.h ../include/*.inl)

qt5_wrap_ui(UI_HEADER_FILES ${UI_FILES})

add_executable(Sandbox3D-Editor ${SRC_FILES} ${UI_FILES})

target_link_libraries(Sandbox3D-Editor PRIVATE sandbox_resources scene Qt5::Core Qt5::Widgets Qt5::Gui Qt5::OpenGL Qt5::Xml GraphWidget Graph)
target_include_directories(Sandbox3D-Editor PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../")

if (CMAKE_BUILD_TYPE MATCHES Debug)
        target_link_libraries(Sandbox3D-Editor PUBLIC glBreak)
endif(CMAKE_BUILD_TYPE MATCHES Debug)
