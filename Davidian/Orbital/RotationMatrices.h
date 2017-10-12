//
// Created by Alex Shaver on 10/11/17.
//

#ifndef DAVIDIAN_ROTATIONMATRICES_H
#define DAVIDIAN_ROTATIONMATRICES_H

#include "Orbit.h"
#include <Eigen/Dense>

namespace orbital {

Eigen::Matrix3d GlobalToOrbit(const Orbit& orbit);

Eigen::Matrix3d OrbitToGlobal(const Orbit& orbit);

} // namespace orbital

#endif //DAVIDIAN_ROTATIONMATRICES_H
