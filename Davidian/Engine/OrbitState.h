//
// Created by Alex Shaver on 10/8/17.
//

#ifndef DAVIDIAN_ORBITSTATE_H
#define DAVIDIAN_ORBITSTATE_H

#include <CartesianVector.h>
#include <GlobalDefinitions.h>
#include <Orbit.h>

namespace engine{

struct OrbitState{
  double time, radius, trueAnomaly;
  orbital::StateVector stateVector;
};

const OrbitState calculateOrbitState(const orbital::Orbit& orbit, double time);

} // namespace engine

#endif //DAVIDIAN_ORBITSTATE_H
