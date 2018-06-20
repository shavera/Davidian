#ifndef ORBITALHISTORY_H
#define ORBITALHISTORY_H

#include "OrbitState.h"

#include "Orbit.h"

namespace engine {

// default precision = 2e6 * pi + 1;
template<size_t precision = 6'283'186>
class OrbitalHistory
{
public:
  explicit OrbitalHistory(const orbital::Orbit& orbit);

  /**
   * @brief Use precalculated history and interpolate between recorded points to derive approximate state vectors
   * @param seconds What time, after epoch, to look for state; will adjust for period internally, so any time is allowed
   * @return StateVector that approximates the position at a given time.
   */
  orbital::StateVector approximateStateAtTime(double seconds);

private:
  const orbital::Orbit m_orbit;

  std::array<OrbitState, precision> m_history;
};

} // namespace engine

#endif // ORBITALHISTORY_H
