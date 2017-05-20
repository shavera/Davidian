//
// Created by alex on 3/17/17.
//

#include "Orbit.h"

namespace orbital {

struct OrbitalAccessories{

};

Orbit::Orbit(const orbital::StateVector& stateVector, double standardGravitationalParameter) {

}

} // namespace orbital


#ifdef BUILD_TESTS

#include <gtest/gtest.h>

namespace orbital{
namespace {

class OrbitTest : public ::testing::Test{

};

TEST_F(OrbitTest, dummy){

}

} // anonymous namespace
} // namespace orbital

#endif // BUILD_TESTS