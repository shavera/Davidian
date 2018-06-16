#ifndef ORBITALHISTORY_H
#define ORBITALHISTORY_H

#include "OrbitState.h"

#include "Orbit.h"

namespace engine {

template<size_t precision = 1e6>
class OrbitalHistory
{
public:
  OrbitalHistory(const orbital::Orbit& orbit);

  OrbitState stateAtTime(double seconds);

private:
  std::array<OrbitState, precision> m_history;
};

} // namespace engine

#endif // ORBITALHISTORY_H
