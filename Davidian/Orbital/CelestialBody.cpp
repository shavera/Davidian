//
// Created by alex on 4/7/17.
//

#include "CelestialBody.h"
#include "Orbit.h"

orbital::CelestialBody::CelestialBody(const double& mass, orbital::CelestialBody* parent,
                                      const orbital::StateVector* initialState, const double& time)
    : Body(mass, parent, initialState, time){}
