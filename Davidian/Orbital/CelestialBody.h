//
// Created by alex on 4/7/17.
//

#ifndef DAVIDIAN_CELESTIALBODY_H
#define DAVIDIAN_CELESTIALBODY_H

#include "Body.h"

namespace orbital{
class CelestialBody : public Body{
public:
  CelestialBody(const double& mass, CelestialBody* parent = nullptr, const StateVector* initialState = nullptr,
                const double& time = 0)
      : Body(mass, parent, initialState, time){}

};
}


#endif //DAVIDIAN_CELESTIALBODY_H
