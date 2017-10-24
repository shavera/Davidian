//
// Created by Alex Shaver on 10/8/17.
//

#include "OrbitalEngine.h"

namespace engine{

std::vector<OrbitState>
OrbitalEngine::generateOrbitalHistory(double numberOfPeriods, int numberOfTimeDivisions, const orbital::Orbit& orbit) {
  std::vector<OrbitState> states(numberOfTimeDivisions,0);
  std::iota(states.begin(), states.end(), 0);
  return states;
}

}

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