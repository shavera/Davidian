//
// Created by alexs on 7/3/18.
//

#include "RotationMatrices.h"

#include <gtest/gtest.h>

namespace orbital {
namespace test {
namespace {

auto ZXZ(const double a, const double b, const double c){
  Eigen::Matrix3d matrix{Eigen::Matrix3d::Zero()};
  const double ca{std::cos(a)}, sa{std::sin(a)}, cb{std::cos(b)}, sb{std::sin(b)}, cc{std::cos(c)}, sc{std::sin(c)};
  matrix(0,0) = ca*cc - cb*sa*sc;
  matrix(0,1) = -ca*sc - cb*cc*sa;
  matrix(0,2) = sa*sb;
  matrix(1,0) = cc*sa + ca*cb*sc;
  matrix(1,1) = ca*cb*cc - sa*sc;
  matrix(1,2) = -ca*sb;
  matrix(2,0) = sb*sc;
  matrix(2,1) = cc*sb;
  matrix(2,2) = cb;
  return matrix;
}

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
        const double calculated{testCalculatedMatrix(i,j)}, actual{actualRotationMatrix(i, j)};
        const double tolerance = (1e-6 > std::fabs(calculated)) ? 1e-6 : std::fabs(1e-6* calculated);
        EXPECT_NEAR(calculated, actual, tolerance) << i << j;
      }
    }
  }

  Eigen::Matrix3d testCalculatedMatrix, actualRotationMatrix{Eigen::Matrix3d::Zero()};
};

TEST_F(RotationMatrices, GlobalToOrbit) {
  testCalculatedMatrix = ZXZ(-argumentOfPeriapsis, -inclination, -longitudeOfAscendingNode);
  actualRotationMatrix = GlobalToOrbit(*orbit);
}

TEST_F(RotationMatrices, OrbitToGlobal) {
  testCalculatedMatrix = ZXZ(longitudeOfAscendingNode, inclination, argumentOfPeriapsis);
  actualRotationMatrix = OrbitToGlobal(*orbit);
}

TEST_F(RotationMatrices, OrbitToGlobal_fromInitialVector){
  testCalculatedMatrix = ZXZ(0, 0, M_PI/6);

  const double M=1.495e30, m=3.333853e27;
  StateVector initialState{{-333833694311, 378875701458, 0},{-11374.999, -8165.466, 0}};
  const Orbit orbit{initialState, M, m};
  actualRotationMatrix = OrbitToGlobal(orbit);
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