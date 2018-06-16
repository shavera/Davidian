#include "OrbitalHistory.h"

namespace engine {

template <size_t precision>
OrbitalHistory<precision>::OrbitalHistory(const orbital::Orbit& orbit)
{
  std::generate(m_history.begin(), m_history.end(), [orbit, m = 0]() mutable {
    double time = orbit.sweep() * m++;
    return calculateOrbitState(orbit, time);
  });
}

template <size_t precision>
OrbitState OrbitalHistory<precision>::stateAtTime(double seconds) {
  return OrbitState();
}

} // namespace engine

#ifdef BUILD_TESTS

#include <gtest/gtest.h>

namespace engine{
namespace {

TEST(OrbitalHistoryTest, simpleOrbit){
  orbital::Orbit simpleOrbit{{{1,0,0}, {0,1,0}}, 1/orbital::G, 0};

  OrbitalHistory simpleHistory{simpleOrbit};

  // for unit circle orbit, period is 2pi, so have a few more than 2pi values to calculate.
  for(size_t s{0}; s < 10; ++s){
    orbital::StateVector expectedState{{std::cos(s), std::sin(s), 0}, {-std::sin(s), std::cos(s), 0}};

    orbital::StateVector actualState{simpleHistory.stateAtTime(s).stateVector};
    for(size_t i{0}; i < 3; ++i){
      EXPECT_NEAR(expectedState.position.c_at(i), actualState.position.c_at(i), 1e-6) << s <<" "<<i;
      EXPECT_NEAR(expectedState.velocity.c_at(i), actualState.velocity.c_at(i), 1e-6) << s <<" "<<i;
    }
  }
}

} // anonymous namespace
} // namespace engine

#endif // BUILD_TESTS