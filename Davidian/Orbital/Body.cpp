//
// Created by alex on 4/7/17.
//

#include "Body.h"

#include "CelestialBody.h"
#include "GlobalDefinitions.h"
#include "Orbit.h"

namespace orbital{

Body::Body(double mass, const OrbitalElements& orbitalElements, const CelestialBody* parent)
  : m_mass{mass}
  , _parent{parent}
  , m_orbit{std::make_unique<Orbit>(orbitalElements, parent->mass(), m_mass)}
{}

Body::Body(const double mass)
  : m_mass{mass}, _parent{nullptr}, m_orbit{nullptr}
{}

double Body::standardGravitationalParameter() const {
  double totalMass = m_mass + ((nullptr != _parent) ? _parent->mass() : 0.0);
  return totalMass*orbital::G;
}

void Body::setParentAndOrbit(const CelestialBody* parent, const OrbitalElements& orbitalElements) {
  _parent = parent;
  if(nullptr != _parent) {
    m_orbit = std::make_unique<Orbit>(orbitalElements, _parent->mass(), m_mass);
  }
}

Body::~Body() = default;

} // namespace orbital