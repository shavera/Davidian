//
// Created by alex on 3/17/17.
//

#ifndef DAVIDIAN_ORBIT_H
#define DAVIDIAN_ORBIT_H

#include "impl/GlobalDefinitions.h"

#include <memory>

namespace orbital{

namespace impl{
class OrbitImpl;
} // namespace impl

class Orbit {
public:
  Orbit(const StateVector& stateVector, double barymass, double leptomass);
  Orbit(const OrbitalElements& elements, double barymass, double leptomass);
  virtual ~Orbit();

  const OrbitalElements& orbitalElements() const;

  inline double energy() const;
  inline const CartesianVector& angularMomentum() const;
  inline double period() const;
  inline double sweep() const;

private:
    std::unique_ptr<impl::OrbitImpl> m_impl;
};

} // namespace orbital


#endif //DAVIDIAN_ORBIT_H
