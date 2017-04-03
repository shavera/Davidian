cmake_minimum_required(VERSION 3.7)
project(davidian)

set(CMAKE_CXX_STANDARD 14)

set(BUILD_SHARED_LIBS ON)

include(CTest)

add_subdirectory(Orbital)

add_subdirectory(3rd_party/eigen-3.3)

if(BUILD_TESTING)
    add_subdirectory(3rd_party/googletest)
    add_definitions(-DTEST)
endif ()

set(SOURCE_FILES main.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})