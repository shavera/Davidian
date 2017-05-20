//
// Created by alex on 3/17/17.
//

#ifndef DAVIDIAN_ORBIT_H
#define DAVIDIAN_ORBIT_H

#include "impl/GlobalDefinitions.h"

namespace orbital{

struct OrbitalElements{
  double semiMajorAxis;
  double eccentricity;
  double inclination;
  double longitudeOfAscendingNode;
  double argumentOfPeriapsis;
  double meanAnomalyAtEpoch;
};

class Orbit {
public:
  Orbit(const StateVector& stateVector, const  );
};
}

} // namespace orbital


#endif //DAVIDIAN_ORBIT_H
