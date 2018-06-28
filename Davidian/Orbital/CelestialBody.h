//
// Created by alex on 4/7/17.
//

#ifndef DAVIDIAN_CELESTIALBODY_H
#define DAVIDIAN_CELESTIALBODY_H

#include "Body.h"

namespace orbital{
class CelestialBody : public Body{
public:
  /**
   * @brief Construct a 'root' Celestial body with no parent or meaningful orbit
   * @param mass Mass, in kg, of this body
   */
  explicit CelestialBody(double mass);

  /**
   * @brief Construct a Celestial body that is in orbit around some other Celestial Body
   * @param mass Mass, in kg, of this body
   * @param elements Orbital Element description of orbit this body occupies around its parent
   * @param parent pointer to the parent body
   */
  CelestialBody(double mass,
                OrbitalElements elements,
                CelestialBody* parent);

};
}


#endif //DAVIDIAN_CELESTIALBODY_H
