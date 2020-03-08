cmake_minimum_required(VERSION 3.14)
project(Sort)

set(CMAKE_CXX_STANDARD 14)

IF (${build} MATCHES "debug")
    set(CMAKE_BUILD_TYPE DEBUG)
    set(CMAKE_CXX_FLAGS "-Wall -Wno-sign-compare -O0 -pthread -m64 -g")
    add_definitions(-DMAC -DGCC -DDEBUG -DDEBUG_LOG)

ELSE(${build} MATCHES "release")
    set(CMAKE_BUILD_TYPE RELEASE)
    set(CMAKE_CXX_FLAGS "-Wall -Wno-sign-compare -O2 -pthread -m64")
    add_definitions(-DMAC -DGCC)
ENDIF(${build} MATCHES "debug")

include_directories(
        ${PROJECT_SOURCE_DIR}/
        /usr/local/include/
)

set(LOG_SRC_FILE
        ${PROJECT_SOURCE_DIR}/Sorting.cc
        ${PROJECT_SOURCE_DIR}/main.cpp
        )
link_directories(/usr/lib /usr/local/lib)
add_executable(Sort ${LOG_SRC_FILE})
target_link_libraries(Sort pthread gtest)
