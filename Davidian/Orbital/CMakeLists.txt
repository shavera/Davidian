cmake_minimum_required(VERSION 3.7)
project(orbital)

set(CMAKE_CXX_STANDARD 14)

set(SOURCES
        Orbit
        Vectors.h
        )

add_library(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${EIGEN_INCLUDE_DIR})
#add_executable(${PROJECT_NAME}_TEST)
#target_link_libraries(${PROJECT_NAME}_TEST gtest_main)