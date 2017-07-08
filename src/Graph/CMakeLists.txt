cmake_minimum_required(VERSION 3.1)

find_library(JANSSON_LIBRARY jansson)

add_library(Graph Graph.cpp Graph.h Node.cpp Node.h Edge.cpp Edge.h MetaDataContainer.cpp MetaDataContainer.h)

target_link_libraries(Graph PUBLIC "${JANSSON_LIBRARY}")
