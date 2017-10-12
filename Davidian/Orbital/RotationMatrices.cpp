//
// Created by Alex Shaver on 10/11/17.
//

#include "RotationMatrices.h"

namespace orbital {
Eigen::Matrix3d GlobalToOrbit(const orbital::Orbit& orbit) {
  return Eigen::Matrix3d();
}

Eigen::Matrix3d OrbitToGlobal(const Orbit& orbit) {
  return Eigen::Matrix3d();
}

} // namespace orbital

#ifdef BUILD_TESTS

#include "impl/OrbitImpl.h"
#include <gtest/gtest.h>

namespace orbital{
namespace test{
namespace {

TEST(RotationMatrices, GlobalToOrbit){
  OrbitalElements elements;
  elements.inclination = 0.1123;
  elements.longitudeOfAscendingNode = 1.0576;
  elements.argumentOfPeriapsis = -0.8742;
  Orbit orbit{elements, 1, 1};
  Eigen::Matrix3d expectedRotationMatrix;
  expectedRotationMatrix << 0, 0, 0,
                            0, 0, 0,
                            0, 0, 0;
//  Eigen::Matrix3d expectedRotationMatrix{
//      {0,0,0},
//      {0,0,0},
//      {0,0,0}
//  };
}

TEST(RotationMatrices, OrbitToGlobal){
//  Eigen::Matrix3d expectedRotationMatrix{
//      {0,0,0},
//      {0,0,0},
//      {0,0,0}
//  };
}

} // anonymous namespace
} // namespace test
} // namespace orbital

#endif //BUILD_TESTS