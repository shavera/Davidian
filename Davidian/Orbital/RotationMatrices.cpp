//
// Created by Alex Shaver on 10/11/17.
//

#include "RotationMatrices.h"

namespace orbital {
Eigen::Matrix3d GlobalToOrbit(const orbital::Orbit& orbit) {
  const OrbitalElements& elements{orbit.orbitalElements()};
  return (Eigen::AngleAxisd(-elements.argumentOfPeriapsis, Eigen::Vector3d::UnitZ()) *
          Eigen::AngleAxisd(-elements.inclination, Eigen::Vector3d::UnitX()) *
          Eigen::AngleAxisd(-elements.longitudeOfAscendingNode, Eigen::Vector3d::UnitZ())).toRotationMatrix();
}

Eigen::Matrix3d OrbitToGlobal(const Orbit& orbit) {
  const OrbitalElements& elements{orbit.orbitalElements()};
  return (Eigen::AngleAxisd(elements.longitudeOfAscendingNode, Eigen::Vector3d::UnitZ()) *
          Eigen::AngleAxisd(elements.inclination, Eigen::Vector3d::UnitX()) *
          Eigen::AngleAxisd(elements.argumentOfPeriapsis, Eigen::Vector3d::UnitZ())).toRotationMatrix();
}

StateVector operator*(const Eigen::Matrix3d& transform, const StateVector& stateVector) {
  StateVector st{transform * stateVector.position, transform * stateVector.velocity};
  return st;
}

} // namespace orbital
