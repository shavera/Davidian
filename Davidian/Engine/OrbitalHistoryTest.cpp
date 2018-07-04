//
// Created by alexs on 7/2/18.
//

#include "OrbitalHistory.h"

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

namespace engine{
namespace {

TEST(OrbitalHistoryTest, simpleOrbit){
  orbital::Orbit simpleOrbit{{{1,0,0}, {0,1,0}}, 1/orbital::G, 0};

  // note: at size ~30, error was ~0.3 percent, at size 1000 error was ~3e-6
  OrbitalHistory simpleHistory{simpleOrbit};

  // for unit circle orbit, period is 2pi, so have a few more than 2pi values to calculate.
  for(double s{0}; s < 10; ++s){
    orbital::StateVector expectedState{{std::cos(s), std::sin(s), 0}, {-std::sin(s), std::cos(s), 0}};

    orbital::StateVector actualState{simpleHistory.approximateStateAtTime(s)};
    EXPECT_LT(expectedState.position.separation(actualState.position), 1e-6)
              << "X " << s << " " << expectedState.position << " " << actualState.position;
    EXPECT_LT(expectedState.velocity.separation(actualState.velocity), 1e-6)
              << "V " << s << " " << expectedState.velocity << " " << actualState.velocity;
  }
}

} // anonymous namespace
} // namespace engine

