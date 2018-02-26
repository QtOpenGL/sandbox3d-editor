# Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Widgets Gui OpenGL Xml REQUIRED)

if (Qt5_POSITION_INDEPENDENT_CODE)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif ()

# add subdirectories
add_subdirectory(GraphWidget)
add_subdirectory(app)
