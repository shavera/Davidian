cmake_minimum_required(VERSION 3.7)
project(davidian)

set(CMAKE_CXX_STANDARD 17)

set(BUILD_SHARED_LIBS ON)

include(CTest)

add_subdirectory(3rd_party/eigen-3.3)

find_package(Protobuf REQUIRED)
find_package(gRPC CONFIG REQUIRED)

if(BUILD_TESTING)
    add_subdirectory(3rd_party/googletest)
    set(GTEST_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/3rd_party/googletest/googletest/include)
    set(GMOCK_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/3rd_party/googletest/googlemock/include)
    add_definitions(-DBUILD_TESTS)
endif ()

add_subdirectory(Orbital)
add_subdirectory(Engine)
add_subdirectory(Proto)

set(SOURCE_FILES main.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
