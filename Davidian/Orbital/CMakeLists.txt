cmake_minimum_required(VERSION 3.7)
project(orbital)

set(CMAKE_CXX_STANDARD 14)

set(SOURCES
        CartesianVector
        Orbit
        SphericalVector

        impl/VectorImpl
        )

add_library(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PUBLIC Eigen)

if (BUILD_TESTING)
    target_include_directories(${PROJECT_NAME} PRIVATE ${GTEST_INCLUDE_DIR})
    add_executable(${PROJECT_NAME}_TEST ${SOURCES})
    target_link_libraries(${PROJECT_NAME}_TEST gtest_main ${PROJECT_NAME})
endif()
