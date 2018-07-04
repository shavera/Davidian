cmake_minimum_required(VERSION 3.7)
project(engine)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
        CelestialSystem
        CelestialSystemFileManager
        OrbitalEngine
        OrbitalHistory
        OrbitState
        )

set(TEST_SOURCES
        CelestialSystemTest
        CSFileManagerTest
        OrbitalEngineTest OrbitalHistoryTest.cpp OrbitStateTest.cpp)

if(BUILD_TESTING)
    list(APPEND SOURCES UnitTestHelpers)
endif()

set(INTERFACES
        EngineInterface.h
        )

add_library(${PROJECT_NAME} ${SOURCES} ${INTERFACES})
target_link_libraries(${PROJECT_NAME} orbital)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if (BUILD_TESTING)
    target_include_directories(${PROJECT_NAME} PRIVATE ${GTEST_INCLUDE_DIR} ${GMOCK_INCLUDE_DIR})
    add_executable(${PROJECT_NAME}_TEST ${TEST_SOURCES})
    target_link_libraries(${PROJECT_NAME}_TEST gmock gmock_main gtest ${PROJECT_NAME})
endif()
