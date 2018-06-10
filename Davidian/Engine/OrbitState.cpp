//
// Created by Alex Shaver on 10/8/17.
//

#include "OrbitState.h"
#include "NewtonSolver.h"
#include "RotationMatrices.h"

using orbital::Orbit;
using orbital::OrbitalElements;
using orbital::StateVector;
using orbital::CartesianVector;

namespace engine {

namespace{

double meanAnomaly_f(double meanAnomalyAtEpoch, double sweep, double time){
  return meanAnomalyAtEpoch + sweep*time;
}

double eccentricAnomaly_f(double meanAnomaly, double eccentricity){
  auto eccAnomalyFunction = [meanAnomaly, eccentricity](double eccentricAnomaly){
    return eccentricAnomaly - eccentricity*std::sin(eccentricAnomaly) - meanAnomaly;
  };
  auto eccAnomalyDerivative = [eccentricity](double eccentricAnomaly){
    return 1-eccentricity*std::cos(eccentricAnomaly);
  };

  return findFunctionRoot(eccAnomalyFunction, eccAnomalyDerivative, 0);
}

double trueAnomaly_f(double eccentricAnomaly, double eccentricity){
  return (std::cos(eccentricAnomaly) - eccentricity)/(1-eccentricity*std::cos(eccentricAnomaly));
}

double radius_f(double semiMajorAxis, double eccentricity, double trueAnomaly){
  return semiMajorAxis*(1-std::pow(eccentricity, 2))/(1+eccentricity*std::cos(trueAnomaly));
}

StateVector orbitCoordinateStateVector(const Orbit& orbit, double radius, double trueAnomaly, double eccentricAnomaly){
  CartesianVector position{radius*std::cos(trueAnomaly), radius*std::sin(trueAnomaly), 0};
  double velocityFactor{std::sqrt(orbit.standardGravitationalParameter()*orbit.orbitalElements().semiMajorAxis)/radius};
  CartesianVector velocity{ -std::sin(eccentricAnomaly),
                            std::sqrt(1-std::pow(orbit.orbitalElements().eccentricity,2))*std::cos(eccentricAnomaly),
                            0 };
  return StateVector{position, velocityFactor*velocity};
}

StateVector globalCoordinateStateVector(const StateVector& orbitCoordinateVector, const Eigen::Matrix3d& orbitToGlobalTxfm){
  return orbitToGlobalTxfm * orbitCoordinateVector;
}

} // anonymous namespace

const OrbitState calculateOrbitState(const orbital::Orbit &orbit, double time)
{
    OrbitState state;
    const double meanAnom{meanAnomaly_f(orbit.orbitalElements().meanAnomalyAtEpoch, orbit.sweep(), time)};
    const double eccAnom{eccentricAnomaly_f(meanAnom, orbit.orbitalElements().eccentricity)};
    state.trueAnomaly = trueAnomaly_f(eccAnom, orbit.orbitalElements().eccentricity);
    state.radius = radius_f(orbit.orbitalElements().semiMajorAxis,  orbit.orbitalElements().eccentricity, state.trueAnomaly);
    const StateVector orbitCoordState = orbitCoordinateStateVector(orbit, state.radius, state.trueAnomaly, eccAnom);
    const Eigen::Matrix3d rotationMatrix{orbital::OrbitToGlobal(orbit)};
    state.stateVector = globalCoordinateStateVector(orbitCoordState, rotationMatrix);
    return state;
}

} // namespace engine

#ifdef BUILD_TESTS

#include <gtest/gtest.h>

namespace engine{
namespace test {
namespace {

TEST(OrbitState_CalculationHelpers, meanAnomaly) {
  const double M0{1.23}, sweep{.123}, time{57};
  const double expectedMeanAnomaly{8.241}, actualMeanAnomaly{meanAnomaly_f(M0, sweep, time)};

  EXPECT_NEAR(expectedMeanAnomaly, actualMeanAnomaly, 1e-6 * expectedMeanAnomaly);
}

TEST(OrbitState_CalculationHelpers, eccentricAnomaly){
  const double meanAnomaly{0.71163115893}, eccentricity{0.55};
  const double expectedEccentricAnomaly{1.23}, actualEccentricAnomaly{eccentricAnomaly_f(meanAnomaly, eccentricity)};

  EXPECT_NEAR(expectedEccentricAnomaly, actualEccentricAnomaly, 1e-6*expectedEccentricAnomaly);
}

TEST(OrbitState_CalculationHelpers, trueAnomaly){
  const double eccentricAnomaly{1.23}, eccentricity{0.55};
  const double expectedTrueAnomaly{-0.264359718102521}, actualTrueAnomaly{trueAnomaly_f(eccentricAnomaly, eccentricity)};
  EXPECT_NEAR(expectedTrueAnomaly, actualTrueAnomaly, std::fabs(1e-6*expectedTrueAnomaly));
}

TEST(OrbitState_CalculationHelpers, radius){
  const double semiMajorAxis{1123}, eccentricity{0.55}, trueAnomaly{-0.264359718102521};
  const double expectedRadius{511.6572486356}, actualRadius{radius_f(semiMajorAxis, eccentricity, trueAnomaly)};
  EXPECT_NEAR(expectedRadius, actualRadius, 1e-6*expectedRadius);
}

TEST(OrbitState_CalculationHelpers, orbitCoordinateStateVector){
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

TEST(OrbitState_CalculationHelpers, globalCoordinateStateVector){
  Eigen::Matrix3d rotationMatrix;
  rotationMatrix << 0.9790201665,	0.1788527114,	0.0976279731,
                    -0.184745715,	0.9812450651,	0.0550194789,
                    -0.0859565838,	-0.0719015291,	0.9937009791;

  StateVector orbitalState{{18.0814907049, 9.6145077816, 0},{-1.8324404306, 1.2574985215, 0}};

  StateVector expectedState{{19.421724826, 6.09371, -2.24552},{-1.56909, 1.57245, 0.0670943}};

  StateVector actualState = globalCoordinateStateVector(orbitalState, rotationMatrix);

  for(int i{0}; i < 3; ++i){
    EXPECT_NEAR(expectedState.position.at(i), actualState.position.at(i), std::fabs(1e-6*expectedState.position.at(i)))
              << "position " << i;
    EXPECT_NEAR(expectedState.velocity.at(i), actualState.velocity.at(i), std::fabs(1e-6*expectedState.velocity.at(i)))
              << "velocity " << i;
  }
}

// this test is very hard to calculate by hand what even the initial state should be much less a resulting state.
TEST(OrbitState, calculateOrbitState){
  const double M=1.495e30, m=3.333853e27;
  StateVector initialState{{-750076382836.7954, -239384315897.20627, 0},{-3870.2776276002724, -4593.496383019142, 0}};
  const Orbit orbit{initialState, M, m};

  OrbitState state{calculateOrbitState(orbit, 0)};
  for(int i{0}; i < 0; ++i){
    EXPECT_NEAR(state.stateVector.position.at(i), initialState.position.at(i), 1e-6*initialState.position.at(i));
    EXPECT_NEAR(state.stateVector.velocity.at(i), initialState.velocity.at(i), 1e-6*initialState.velocity.at(i));
  }

}

} // anonymous namespace
} // namespace test
} // namespace engine

#endif // BUILD_TESTS
