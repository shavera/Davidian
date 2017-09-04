//
// Created by alex on 4/7/17.
//

#ifndef DAVIDIAN_GLOBALDEFINITIONS_H
#define DAVIDIAN_GLOBALDEFINITIONS_H

#include "../CartesianVector.h"

namespace orbital{

struct StateVector{
  CartesianVector position, velocity;
};

constexpr double G{6.67408e-11};

inline double massParameter(const double& mass){return G*mass;}

struct OrbitalElements{
    double semiMajorAxis{0.0};
    double eccentricity{0.0};
    double inclination{0.0};
    double longitudeOfAscendingNode{0.0};
    double argumentOfPeriapsis{0.0};
    double meanAnomalyAtEpoch{0.0};
};

} // namespace orbital

#endif //DAVIDIAN_GLOBALDEFINITIONS_H
