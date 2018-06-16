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
  auto bound = std::upper_bound(m_history.cbegin(), m_history.cend(), seconds,
                                [](const double& time, const OrbitState& state){ return time < state.time; });
  return *bound;
}

} // namespace engine

#ifdef BUILD_TESTS

#include <gtest/gtest.h>

namespace engine{
namespace {

TEST(OrbitalHistoryTest, simpleOrbit){
  orbital::Orbit simpleOrbit{{{1,0,0}, {0,1,0}}, 1/orbital::G, 0};

  OrbitalHistory<10> simpleHistory{simpleOrbit};

  // for unit circle orbit, period is 2pi, so have a few more than 2pi values to calculate.
  for(double s{0}; s < 10; ++s){
    orbital::StateVector expectedState{{std::cos(s), std::sin(s), 0}, {-std::sin(s), std::cos(s), 0}};

    orbital::StateVector actualState{simpleHistory.stateAtTime(s).stateVector};
    EXPECT_LT(expectedState.position.separation(actualState.position), 1e-6)
              << "X " << s << " " << expectedState.position << " " << actualState.position;
    EXPECT_LT(expectedState.velocity.separation(actualState.velocity), 1e-6)
              << "V " << s << " " << expectedState.velocity << " " << actualState.velocity;
  }
}

} // anonymous namespace
} // namespace engine

#endif // BUILD_TESTS