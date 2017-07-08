cmake_minimum_required(VERSION 3.1)

set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5OpenGL)

if(Qt5_POSITION_INDEPENDENT_CODE)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

set(SOURCES_VIEW View.cpp View.h ConnectionHandler.cpp ConnectionHandler.h )
set(SOURCES_NODE Node.cpp Node.h NodeTexture.cpp NodeTexture.h NodePass.cpp NodePass.h NodeFinal.cpp NodeFinal.h)
set(SOURCES_CONNECTOR Connector.cpp Connector.h ConnectorInput.cpp ConnectorInput.h ConnectorOutput.cpp ConnectorOutput.h)
set(SOURCES_EDGE Edge.cpp Edge.h)

add_library(GraphWidget ${SOURCES_VIEW} ${SOURCES_NODE} ${SOURCES_CONNECTOR} ${SOURCES_EDGE})

target_link_libraries(GraphWidget PUBLIC Qt5::Core Qt5::Gui Qt5::OpenGL)
