//
// Created by Alex Shaver on 10/8/17.
//

#ifndef DAVIDIAN_ORBITSTATE_H
#define DAVIDIAN_ORBITSTATE_H

#include <CartesianVector.h>
#include <impl/GlobalDefinitions.h>
#include <Orbit.h>

namespace engine{

struct OrbitState{
  OrbitState(const orbital::Orbit& orbit, double time);
  double time, trueAnomaly;
  orbital::StateVector stateVector;
};

} // namespace engine

#endif //DAVIDIAN_ORBITSTATE_H
