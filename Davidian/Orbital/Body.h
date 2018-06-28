//
// Created by alex on 4/7/17.
//

#ifndef DAVIDIAN_BODY_H
#define DAVIDIAN_BODY_H

#include <memory>
#include "GlobalDefinitions.h"

namespace orbital{

class CelestialBody;
class Orbit;
class StateVector;

class Body {
public:
//  Body(const double& mass, CelestialBody* parent = nullptr, const StateVector* initialState = nullptr,
//       const double& time = 0);

  Body(const double& mass, const OrbitalElements& orbitalElements, CelestialBody* parent);
  virtual ~Body();

  /// Using the more precise definition of GM_1 + GM_2, rather than just approximating it to be GM_1 (as appropriate)
  double standardGravitationalParameter() const;
  /// Just the mass of this body.
  double mass() const;

private:
  // allowing mass to be mutable if we wish to do more general cases like rockets that lose mass during burns.
  double m_mass;
  CelestialBody* _parent;
  std::unique_ptr<Orbit> m_orbit;

};

} // namespace orbital

#endif //DAVIDIAN_BODY_H
