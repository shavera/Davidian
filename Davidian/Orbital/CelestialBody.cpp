//
// Created by alex on 4/7/17.
//

#include "CelestialBody.h"
#include "Orbit.h"

orbital::CelestialBody::CelestialBody(const double mass)
  : Body{mass}
{}

orbital::CelestialBody::CelestialBody(const double mass,
                                      OrbitalElements elements,
                                      CelestialBody* parent)
  : Body{mass, elements, parent}
{}
