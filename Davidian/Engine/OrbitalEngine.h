//
// Created by Alex Shaver on 10/8/17.
//

#ifndef DAVIDIAN_ORBITALENGINE_H
#define DAVIDIAN_ORBITALENGINE_H

#include "OrbitalHistory.h"
#include "OrbitState.h"
#include <CartesianVector.h>

namespace engine {

class OrbitalEngine{
public:
  OrbitalHistory generateOrbitalHistory(const orbital::Orbit& orbit);

};

} // namespace engine

#endif //DAVIDIAN_ORBITALENGINE_H
