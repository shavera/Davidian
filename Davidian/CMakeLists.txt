cmake_minimum_required(VERSION 3.7)
project(davidian)

set(CMAKE_CXX_STANDARD 14)

set(BUILD_SHARED_LIBS ON)

include(CTest)

add_subdirectory(3rd_party/eigen-3.3)

if(BUILD_TESTING)
    add_subdirectory(3rd_party/googletest)
    set(GTEST_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/3rd_party/googletest/googletest/include)
    set(GMOCK_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/3rd_party/googletest/googlemock/include)
    add_definitions(-DBUILD_TESTS)
endif ()

add_subdirectory(Orbital)


set(SOURCE_FILES main.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})