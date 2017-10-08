//
// Created by alex on 3/17/17.
//

#include "Orbit.h"
#include "impl/OrbitImpl.h"

namespace orbital {

Orbit::Orbit(const StateVector& stateVector, double barymass, double leptomass, impl::OrbitImpl* impl)
    : m_impl{nullptr == impl ? new impl::OrbitImpl{stateVector, barymass, leptomass} : impl}
{

}

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

Orbit::~Orbit() = default;

} // namespace orbital