//
// Created by Alex Shaver on 10/8/17.
//

#include "OrbitState.h"

namespace engine {

namespace{



} // anonymous namespace

OrbitState::OrbitState(const orbital::Orbit& orbit, double time) {

}

} // namespace engine

#ifdef BUILD_TESTS

#include <gtest/gtest.h>

namespace orbital{
namespace {

class OrbitTest : public ::testing::Test{

};

TEST_F(OrbitTest, dummy){
//    Orbit orbit(StateVector{}, 1);
}

} // anonymous namespace
} // namespace orbital

#endif // BUILD_TESTS