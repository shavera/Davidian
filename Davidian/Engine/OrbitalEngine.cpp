//
// Created by Alex Shaver on 10/8/17.
//

#include "OrbitalEngine.h"

#ifdef BUILD_TESTS

#include <gtest/gtest.h>

namespace engine{
namespace {

class OrbitTest : public ::testing::Test{

};

TEST_F(OrbitTest, dummy){
//    Orbit orbit(StateVector{}, 1);
}

} // anonymous namespace
} // namespace engine

#endif // BUILD_TESTS