#ifndef ORBITALHISTORY_H
#define ORBITALHISTORY_H

#include "dynarray.h"
#include "OrbitState.h"

#include "Orbit.h"

namespace engine {

class OrbitalHistory
{
public:
  OrbitalHistory(const orbital::Orbit& orbit);

  OrbitState stateAtTime(size_t seconds);

private:
  dynarray<OrbitState> m_history;
};

} // namespace engine

#endif // ORBITALHISTORY_H
