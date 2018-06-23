cmake_minimum_required(VERSION 3.7)
project(engine)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
        CelestialSystemFileManager
        OrbitalEngine
        OrbitalHistory
        OrbitState
        )

set(INTERFACES
        EngineInterface.h)

add_library(${PROJECT_NAME} ${SOURCES} ${INTERFACES})
target_link_libraries(${PROJECT_NAME} orbital)

if (BUILD_TESTING)
    target_include_directories(${PROJECT_NAME} PRIVATE ${GTEST_INCLUDE_DIR} ${GMOCK_INCLUDE_DIR})
    add_executable(${PROJECT_NAME}_TEST ${SOURCES})
    target_link_libraries(${PROJECT_NAME}_TEST gmock gmock_main gtest ${PROJECT_NAME})
endif()
