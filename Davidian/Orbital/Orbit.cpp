//
// Created by alex on 3/17/17.
//

#include "Orbit.h"
#include "impl/OrbitImpl.h"

namespace orbital {

Orbit::Orbit(const StateVector& stateVector, double barymass, double leptomass)
  : m_impl{new impl::OrbitImpl{stateVector, barymass, leptomass}}
{}

Orbit::Orbit(const OrbitalElements& elements, double barymass, double leptomass)
  : m_impl{new impl::OrbitImpl{}}
{
  m_impl->m_elements = elements;
  const double stdGravParam{impl::OrbitImpl::standardGravitationalParameter(barymass, leptomass)};
  m_impl->m_specificOrbitalEnergy = impl::OrbitImpl::energyFromElements(m_impl->m_elements.semiMajorAxis, stdGravParam);
  m_impl->m_period = impl::OrbitImpl::period(m_impl->m_elements.semiMajorAxis, stdGravParam);
  m_impl->m_sweep = impl::OrbitImpl::sweep(m_impl->m_period);
}

Orbit::~Orbit() = default;

double Orbit::energy() const {
  return m_impl->m_specificOrbitalEnergy;
}

const CartesianVector& Orbit::angularMomentum() const {
  return m_impl->m_specificAngularMomentum;
}

double Orbit::period() const {
  return m_impl->m_period;
}

double Orbit::sweep() const {
  return m_impl->m_sweep;
}

const OrbitalElements& Orbit::orbitalElements() const {
  return m_impl->m_elements;
}

} // namespace orbital