cmake_minimum_required(VERSION 3.7)
project(orbital)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
        Body
        CartesianVector
        CelestialBody
        NewtonSolver
        Orbit
        SphericalVector
        RotationMatrices)

add_library(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC Eigen davidian_proto)

if (BUILD_TESTING)
    target_include_directories(${PROJECT_NAME} PRIVATE ${GTEST_INCLUDE_DIR})
    add_executable(${PROJECT_NAME}_TEST ${SOURCES})
    target_link_libraries(${PROJECT_NAME}_TEST gmock_main gtest ${PROJECT_NAME})
endif()
