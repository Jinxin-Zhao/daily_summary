cmake_minimum_required(VERSION 3.14)
project(Algorithm)

set(CMAKE_CXX_STANDARD 17)

include_directories(
        ${PROJECT_SOURCE_DIR}/
        /usr/local/include/
)

set(LOG_SRC_FILE
        ${PROJECT_SOURCE_DIR}/algorithm.cc
        ${PROJECT_SOURCE_DIR}/proxy.cc
        ${PROJECT_SOURCE_DIR}/main.cpp
        )

add_executable(Algorithm ${LOG_SRC_FILE})
target_link_libraries(Algorithm pthread gtest glog)
