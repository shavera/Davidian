//
// Created by Alex Shaver on 10/11/17.
//

#include "RotationMatrices.h"

namespace orbital {
Eigen::Matrix3d GlobalToOrbit(const orbital::Orbit& orbit) {
  const OrbitalElements& elements{orbit.orbitalElements()};
  return (Eigen::AngleAxisd(elements.argumentOfPeriapsis, Eigen::Vector3d::UnitZ()) *
      Eigen::AngleAxisd(elements.inclination, Eigen::Vector3d::UnitX()) *
      Eigen::AngleAxisd(elements.longitudeOfAscendingNode, Eigen::Vector3d::UnitZ())).toRotationMatrix();
}

Eigen::Matrix3d OrbitToGlobal(const Orbit& orbit) {
  const OrbitalElements& elements{orbit.orbitalElements()};
  return (Eigen::AngleAxisd(-elements.longitudeOfAscendingNode, Eigen::Vector3d::UnitZ()) *
          Eigen::AngleAxisd(-elements.inclination, Eigen::Vector3d::UnitX()) *
          Eigen::AngleAxisd(-elements.argumentOfPeriapsis, Eigen::Vector3d::UnitZ())).toRotationMatrix();
}

} // namespace orbital

#ifdef BUILD_TESTS

#include "impl/OrbitImpl.h"
#include <gtest/gtest.h>

namespace orbital{
namespace test{
namespace {

class RotationMatrices : public testing::Test{
public:
  void SetUp() override {
    elements.inclination = inclination;
    elements.longitudeOfAscendingNode = longitudeOfAscendingNode;
    elements.argumentOfPeriapsis = argumentOfPeriapsis;
    orbit = std::make_unique<Orbit>(elements, 1, 1);
  }

  void TearDown() override{
    testMatrices();
  }
  OrbitalElements elements;
  const double inclination{0.1123}, longitudeOfAscendingNode{1.0576}, argumentOfPeriapsis{-0.8742};
  std::unique_ptr<Orbit> orbit;

  void testMatrices(){
    for(int i{0}; i < 3; ++i){
      for(int j{0}; j < 3; ++j){
        const double expected{expectedRotationMatrix(i, j)}, actual{actualRotationMatrix(i, j)};
        EXPECT_NEAR(expected, actual, std::fabs(1e-6*expected)) << i << j;
      }
    }
  }

  Eigen::Matrix3d expectedRotationMatrix, actualRotationMatrix{Eigen::Matrix3d::Zero()};
};

TEST_F(RotationMatrices, GlobalToOrbit){
  expectedRotationMatrix << 0.9790201665,	-0.184745715,	-0.0859565838,
                            0.1788527114,	0.9812450651,	-0.0719015291,
                            0.0976279731,	0.0550194789,	0.9937009791;
  actualRotationMatrix = GlobalToOrbit(*orbit);
}

TEST_F(RotationMatrices, OrbitToGlobal){
  expectedRotationMatrix << 0.9790201665,	0.1788527114,	0.0976279731,
                            -0.184745715,	0.9812450651,	0.0550194789,
                            -0.0859565838,	-0.0719015291,	0.9937009791;
  actualRotationMatrix = OrbitToGlobal(*orbit);
}

} // anonymous namespace
} // namespace test
} // namespace orbital

#endif //BUILD_TESTS