//
// Created by alex on 4/7/17.
//

#include "CelestialBody.h"
#include "Orbit.h"

orbital::CelestialBody::CelestialBody(const double mass)
  : Body{mass}
{}

orbital::CelestialBody::CelestialBody(double mass,
                                      OrbitalElements elements,
                                      const CelestialBody* parent)
  : Body{mass, elements, parent}
{}
