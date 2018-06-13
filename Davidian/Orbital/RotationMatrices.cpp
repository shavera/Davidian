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

StateVector operator*(const Eigen::Matrix3d& transform, const StateVector& stateVector) {
  StateVector st{transform * stateVector.position, transform * stateVector.velocity};
  return st;
}

} // namespace orbital

#ifdef BUILD_TESTS

#include <gtest/gtest.h>

namespace orbital {
namespace test {
namespace {

class RotationMatrices : public testing::Test {
public:
  void SetUp() override {
    elements.inclination = inclination;
    elements.longitudeOfAscendingNode = longitudeOfAscendingNode;
    elements.argumentOfPeriapsis = argumentOfPeriapsis;
    orbit = std::make_unique<Orbit>(elements, 1, 1);
  }

  void TearDown() override {
    testMatrices();
  }

  OrbitalElements elements;
  const double inclination{0.1123}, longitudeOfAscendingNode{1.0576}, argumentOfPeriapsis{-0.8742};
  std::unique_ptr<Orbit> orbit;

  void testMatrices() {
    for(int i{0}; i < 3; ++i) {
      for(int j{0}; j < 3; ++j) {
        const double expected{expectedRotationMatrix(i, j)}, actual{actualRotationMatrix(i, j)};
        EXPECT_NEAR(expected, actual, std::fabs(1e-6 * expected)) << i << j;
      }
    }
  }

  Eigen::Matrix3d expectedRotationMatrix, actualRotationMatrix{Eigen::Matrix3d::Zero()};
};

TEST_F(RotationMatrices, GlobalToOrbit) {
  expectedRotationMatrix << 0.9790201665, -0.184745715, -0.0859565838,
      0.1788527114, 0.9812450651, -0.0719015291,
      0.0976279731, 0.0550194789, 0.9937009791;
  actualRotationMatrix = GlobalToOrbit(*orbit);
}

TEST_F(RotationMatrices, OrbitToGlobal) {
  expectedRotationMatrix << 0.9790201665, 0.1788527114, 0.0976279731,
      -0.184745715, 0.9812450651, 0.0550194789,
      -0.0859565838, -0.0719015291, 0.9937009791;
  actualRotationMatrix = OrbitToGlobal(*orbit);
}

TEST(RotationMatrixOperation, StateVector) {
  Eigen::Matrix3d rotationMatrix;
  rotationMatrix << 0.9790201665, 0.1788527114, 0.0976279731,
      -0.184745715, 0.9812450651, 0.0550194789,
      -0.0859565838, -0.0719015291, 0.9937009791;

  const StateVector initialState{{18.0814907049, 9.6145077816, 0},
                                 {-1.8324404306, 1.2574985215, 0}};

  const StateVector expectedState{{19.421724826, 6.09371, -2.24552},
                                  {-1.56909,     1.57245, 0.0670943}};

  const StateVector actualState = rotationMatrix * initialState;

  for(int i{0}; i < 3; ++i) {
    EXPECT_NEAR(expectedState.position.c_at(i), actualState.position.c_at(i),
                std::fabs(1e-6 * expectedState.position.c_at(i)))
              << "position " << i;
    EXPECT_NEAR(expectedState.velocity.c_at(i), actualState.velocity.c_at(i),
                std::fabs(1e-6 * expectedState.velocity.c_at(i)))
              << "velocity " << i;
  }
}

TEST(RotationMatrixOperation, StateVector2) {
  Eigen::Matrix3d rotationMatrix;
  rotationMatrix << std::sqrt(3) / 2, -0.5, 0,
      0.5, std::sqrt(3) / 2, 0,
      0, 0, 1;

  const StateVector initialState{{-99670645342.849289, 495032822214.40802,  0},
                                 {-13933.771000929162, -1384.0025075203328, 0}};

  const StateVector expectedState{{-333833694311, 378875701458, 0},
                                  {-11374.999,    -8165.466,    0}};

  const StateVector actualState{rotationMatrix * initialState};

  for(int i{0}; i < 3; ++i) {
    EXPECT_NEAR(expectedState.position.c_at(i), actualState.position.c_at(i),
                std::fabs(1e-6 * expectedState.position.c_at(i)))
              << "position " << i;
    EXPECT_NEAR(expectedState.velocity.c_at(i), actualState.velocity.c_at(i),
                std::fabs(1e-6 * expectedState.velocity.c_at(i)))
              << "velocity " << i;
  }
}

} // anonymous namespace
} // namespace test
} // namespace orbital

#endif //BUILD_TESTS
