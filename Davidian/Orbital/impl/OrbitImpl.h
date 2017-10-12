//
// Created by alexs on 5/19/17.
//

#ifndef DAVIDIAN_ORBITIMPL_H
#define DAVIDIAN_ORBITIMPL_H

#include "GlobalDefinitions.h"

namespace orbital{
namespace impl{

struct OrbitImpl {
  OrbitImpl(const StateVector& stateVector, double barymass, double leptomass);
  OrbitImpl() = default;

  static double standardGravitationalParameter(double m1, double m2);
  static double energyFromElements(double semiMajorAxis, double standardGravitationalParameter);
  static double period(double semiMajorAxis, double standardGravitationalParameter);
  static double sweep(double period);

  orbital::OrbitalElements m_elements;
  double m_specificOrbitalEnergy{0}, m_period{0}, m_sweep{0};
  CartesianVector m_specificAngularMomentum;
};

} // namespace impl
} // namespace orbital

#endif //DAVIDIAN_ORBITIMPL_H
