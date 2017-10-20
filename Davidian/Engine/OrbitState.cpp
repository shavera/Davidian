//
// Created by Alex Shaver on 10/8/17.
//

#include "OrbitState.h"
#include "NewtonSolver.h"

using orbital::Orbit;
using orbital::OrbitalElements;
using orbital::StateVector;
using orbital::CartesianVector;

namespace engine {

namespace{

double meanAnomaly(double meanAnomalyAtEpoch, double sweep, double time){
  return meanAnomalyAtEpoch + sweep*time;
}

double eccentricAnomaly(double meanAnomaly, double eccentricity){
  auto eccAnomalyFunction = [meanAnomaly, eccentricity](double eccentricAnomaly){
    return eccentricAnomaly - eccentricity*std::sin(eccentricAnomaly) - meanAnomaly;
  };
  auto eccAnomalyDerivative = [eccentricity](double eccentricAnomaly){
    return 1-eccentricity*std::cos(eccentricAnomaly);
  };

  return findFunctionRoot(eccAnomalyFunction, eccAnomalyDerivative, 0);
}

double trueAnomaly(double eccentricAnomaly, double eccentricity){
  return (std::cos(eccentricAnomaly) - eccentricity)/(1-eccentricity*std::cos(eccentricAnomaly));
}

double radius(double semiMajorAxis, double eccentricity, double trueAnomaly){
  return semiMajorAxis*(1-std::pow(eccentricity, 2))/(1+eccentricity*std::cos(trueAnomaly));
}

StateVector orbitCoordinateStateVector(const Orbit& orbit, double radius, double trueAnomaly, double eccentricAnomaly){
  CartesianVector position{radius*std::cos(trueAnomaly), radius*std::sin(trueAnomaly), 0};
  double velocityFactor{std::sqrt(orbit.standardGravitationalParameter()*orbit.orbitalElements().semiMajorAxis)/radius};
  CartesianVector velocity{ -std::sin(eccentricAnomaly),
                            std::sqrt(1-std::pow(orbit.orbitalElements().eccentricity,2))*std::cos(eccentricAnomaly),
                            0 };
  return StateVector{position, velocityFactor*velocity};
//  return StateVector{};
}

} // anonymous namespace

OrbitState::OrbitState(const orbital::Orbit& orbit, double time) {

}

} // namespace engine

#ifdef BUILD_TESTS

#include <gtest/gtest.h>

namespace engine{
namespace test {
namespace {

class OrbitState : public ::testing::Test {

};

TEST_F(OrbitState, meanAnomaly) {
  const double M0{1.23}, sweep{.123}, time{57};
  const double expectedMeanAnomaly{8.241}, actualMeanAnomaly{meanAnomaly(M0, sweep, time)};

  EXPECT_NEAR(expectedMeanAnomaly, actualMeanAnomaly, 1e-6 * expectedMeanAnomaly);
}

TEST_F(OrbitState, eccentricAnomaly){
  const double meanAnomaly{0.71163115893}, eccentricity{0.55};
  const double expectedEccentricAnomaly{1.23}, actualEccentricAnomaly{eccentricAnomaly(meanAnomaly, eccentricity)};

  EXPECT_NEAR(expectedEccentricAnomaly, actualEccentricAnomaly, 1e-6*expectedEccentricAnomaly);
}

TEST_F(OrbitState, trueAnomaly){
  const double eccentricAnomaly{1.23}, eccentricity{0.55};
  const double expectedTrueAnomaly{-0.264359718102521}, actualTrueAnomaly{trueAnomaly(eccentricAnomaly, eccentricity)};
  EXPECT_NEAR(expectedTrueAnomaly, actualTrueAnomaly, std::fabs(1e-6*expectedTrueAnomaly));
}

TEST_F(OrbitState, radius){
  const double semiMajorAxis{1123}, eccentricity{0.55}, trueAnomaly{-0.264359718102521};
  const double expectedRadius{511.6572486356}, actualRadius{radius(semiMajorAxis, eccentricity, trueAnomaly)};
  EXPECT_NEAR(expectedRadius, actualRadius, 1e-6*expectedRadius);
}

TEST_F(OrbitState, orbitCoordinateStateVector){
  OrbitalElements elements;
  elements.semiMajorAxis = 22.7429;
  elements.eccentricity = 0.11;
  Orbit orbit{elements, 1.3e12, 7e10};
  const double eccAnom{0.966520515}, trueAnom{0.488709809}, r{20.47874669};
  StateVector expectedState{{18.0814907049, 9.6145077816, 0},{-1.8324404306, 1.2574985215, 0}};
  StateVector actualState = orbitCoordinateStateVector(orbit,r, trueAnom, eccAnom);
  for(int i{0}; i < 2; ++i){
    EXPECT_NEAR(expectedState.position.at(i), actualState.position.at(i), std::fabs(1e-6*expectedState.position.at(i)))
              << "position " << i;
    EXPECT_NEAR(expectedState.velocity.at(i), actualState.velocity.at(i), std::fabs(1e-6*expectedState.velocity.at(i)))
              << "velocity " << i;
  }
  EXPECT_EQ(0, actualState.position.z());
  EXPECT_EQ(0, actualState.velocity.z());
}

} // anonymous namespace
} // namespace test
} // namespace engine

#endif // BUILD_TESTS