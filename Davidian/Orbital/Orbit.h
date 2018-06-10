//
// Created by alex on 3/17/17.
//

#ifndef DAVIDIAN_ORBIT_H
#define DAVIDIAN_ORBIT_H

#include "impl/GlobalDefinitions.h"

namespace orbital{

class Orbit {
public:
  Orbit(const StateVector& stateVector, double barymass, double leptomass);
  Orbit(const OrbitalElements& elements, double barymass, double leptomass);
  virtual ~Orbit();

  const OrbitalElements& orbitalElements() const {return m_elements;}

  double standardGravitationalParameter() const {return m_standardGravitationalParameter;}
  double energy() const {return m_specificOrbitalEnergy;}
  const CartesianVector& angularMomentum() const {return m_specificAngularMomentum;}
  double period() const {return m_period;}
  double sweep() const {return m_sweep;}
  const StateVector& initialVector() const {return initialVector();}

private:
  orbital::OrbitalElements m_elements;
  double m_standardGravitationalParameter{0}, m_specificOrbitalEnergy{0}, m_period{0}, m_sweep{0};
  CartesianVector m_specificAngularMomentum;
  StateVector m_initialVector;
};

} // namespace orbital


#endif //DAVIDIAN_ORBIT_H
