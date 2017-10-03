//
// Created by alexs on 5/19/17.
//

#ifndef DAVIDIAN_ORBITIMPL_H
#define DAVIDIAN_ORBITIMPL_H

#include "GlobalDefinitions.h"

namespace orbital{
namespace impl{

class OrbitImpl {
public:
  OrbitImpl(const StateVector& stateVector, const double barymass, const double leptomass);

  orbital::OrbitalElements m_elements;
  double m_specificOrbitalEnergy;
  CartesianVector m_specificAngularMomentum, m_eccentricityVector;
};

} // namespace impl
} // namespace orbital

#endif //DAVIDIAN_ORBITIMPL_H
