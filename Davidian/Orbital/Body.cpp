//
// Created by alex on 4/7/17.
//

#include "Body.h"

orbital::Body::Body(const double& mass, orbital::CelestialBody* parent, orbital::StateVector* intitalState,
                    const double& time) {

}

const double orbital::Body::standardGravitationalParameter() const {
  return 0;
}

const double orbital::Body::mass() const {
  return 0;
}
