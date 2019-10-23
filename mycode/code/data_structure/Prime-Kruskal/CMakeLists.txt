cmake_minimum_required(VERSION 3.14)
project(Prime_Kruskal)

set(CMAKE_CXX_STANDARD 14)

include_directories(
        ${PROJECT_SOURCE_DIR}
)

set(GRAPH_SRC_FILE
        ${PROJECT_SOURCE_DIR}/Floyd-Dijkstra.cc
        ${PROJECT_SOURCE_DIR}/Prime-Kruskal.cc
        ${PROJECT_SOURCE_DIR}/main.cpp
        )

add_executable(Prime_Kruskal ${GRAPH_SRC_FILE})