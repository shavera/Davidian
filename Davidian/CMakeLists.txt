cmake_minimum_required(VERSION 3.7)
project(davidian)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(Orbital)

set(SOURCE_FILES main.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})