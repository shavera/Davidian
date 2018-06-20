#ifndef ORBITALHISTORY_H
#define ORBITALHISTORY_H

#include "OrbitState.h"

#include "Orbit.h"

namespace engine {

/**
 * @brief OrbitalHistory is a calculated-on-construction class that generates a series of Orbit states as a function of
 * time around an orbit. It provides a utility to find approximate state vectors (interpolated between these
 * pre-calculated states). In initial testing, in a circular orbit, 5000 points was sufficient to get 1 part in a
 * million accuracy.
 * @tparam precision Number of bins to calculate. Default is 2^13, approximately 2pi * 1e4, so approximately .0001 rad
 */
template<size_t precision = 65536>
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
