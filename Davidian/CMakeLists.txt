cmake_minimum_required(VERSION 3.7)
project(davidian)

set(CMAKE_CXX_STANDARD 17)

set(BUILD_SHARED_LIBS ON)

include(CTest)

add_subdirectory(3rd_party/eigen-3.3)

set(protobuf_MODULE_COMPATIBLE true)
find_package(Protobuf REQUIRED)
message(STATUS "Using protobuf ${Protobuf_VERSION}")

find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")

if(BUILD_TESTING)
    add_subdirectory(3rd_party/googletest)
    set(GTEST_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/3rd_party/googletest/googletest/include)
    set(GMOCK_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/3rd_party/googletest/googlemock/include)
    add_definitions(-DBUILD_TESTS)
endif ()

add_subdirectory(Orbital)
add_subdirectory(Engine)
add_subdirectory(Proto)
add_subdirectory(Server)

set(SOURCE_FILES main.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} server)

if(BUILD_TESTING)
    target_link_libraries(${PROJECT_NAME} gmock gtest)
endif()
