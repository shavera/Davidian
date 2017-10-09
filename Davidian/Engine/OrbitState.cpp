//
// Created by Alex Shaver on 10/8/17.
//

#include "OrbitState.h"

namespace engine {

namespace{

double meanAnomaly(double meanAnomalyAtEpoch, double sweep, double time){
  return meanAnomalyAtEpoch + sweep*time;
}

} // anonymous namespace

OrbitState::OrbitState(const orbital::Orbit& orbit, double time) {

}

} // namespace engine

#ifdef BUILD_TESTS

#include <gtest/gtest.h>

namespace engine{
namespace test {
namespace {

class OrbitState : public ::testing::Test {

};

TEST_F(OrbitState, meanAnomaly) {
  const double M0{1.23}, sweep{.123}, time{57};
  const double expectedMeanAnomaly{8.241}, actualMeanAnomaly{meanAnomaly(M0, sweep, time)};

  EXPECT_NEAR(expectedMeanAnomaly, actualMeanAnomaly, 1e-6 * expectedMeanAnomaly);
}

} // anonymous namespace
} // namespace test
} // namespace engine

#endif // BUILD_TESTS