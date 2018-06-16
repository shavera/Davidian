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

  OrbitState stateAtTime(double seconds);

private:
  std::array<OrbitState, precision> m_history;
};

} // namespace engine

#endif // ORBITALHISTORY_H
