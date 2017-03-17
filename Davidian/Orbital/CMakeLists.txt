cmake_minimum_required(VERSION 3.7)
project(orbital)

set(CMAKE_CXX_STANDARD 14)

set(SOURCES Orbit)

add_library(${PROJECT_NAME} ${SOURCES})