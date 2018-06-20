cmake_minimum_required(VERSION 3.7)
project(engine)

set(CMAKE_CXX_STANDARD 17)



set(SOURCES
        OrbitalEngine
        OrbitalHistory
        OrbitState
        )

include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS Engine.proto)

add_library(${PROJECT_NAME} ${SOURCES} ${PROTO_SRCS})
target_link_libraries(${PROJECT_NAME} orbital ${Protobuf_LIBRARIES})

if (BUILD_TESTING)
    target_include_directories(${PROJECT_NAME} PRIVATE ${GTEST_INCLUDE_DIR})
    add_executable(${PROJECT_NAME}_TEST ${SOURCES})
    target_link_libraries(${PROJECT_NAME}_TEST gmock_main gtest ${PROJECT_NAME})
endif()
