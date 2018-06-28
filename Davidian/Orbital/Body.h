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
  /**
   * Constructing a Generic body that is in orbit around some other body. The parent and orbit may change as the system
   * evolves over time.
   * @param mass Mass, in kg, of this body
   * @param orbitalElements Description of (initial) orbit around (initial) parent body using orbital elements
   * @param parent Pointer to the (Celestial) body this body orbits.
   */
  Body(double mass, const OrbitalElements& orbitalElements, CelestialBody* parent);
  virtual ~Body();

  /// Using the more precise definition of GM_1 + GM_2, rather than just approximating it to be GM_1 (as appropriate)
  double standardGravitationalParameter() const;
  /// Just the mass of this body.
  double mass() const;

protected:
  /**
   * Construct a 'root' parent body that is at the origin of the coordinate system and does not orbit or move.
   * Protected because CelestialBody subclass is what needs access to it.
   * @param mass
   */
  explicit Body(double mass);

private:
  // allowing mass to be mutable if we wish to do more general cases like rockets that lose mass during burns.
  double m_mass;
  CelestialBody* _parent;
  std::unique_ptr<Orbit> m_orbit;

};

} // namespace orbital

#endif //DAVIDIAN_BODY_H
