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

double trueAnomaly_f(double eccentricAnomaly, double eccentricity){
  return 2*std::atan2(std::sqrt(1+eccentricity)*std::sin(eccentricAnomaly/2),
                      std::sqrt(1-eccentricity)*std::cos(eccentricAnomaly/2));
}

double radius_f(double semiMajorAxis, double eccentricity, double eccentricAnomaly){
  return semiMajorAxis*(1-eccentricity*std::cos(eccentricAnomaly));
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
    const auto& elements = orbit.orbitalElements();
    const double meanAnom{meanAnomaly_f(elements.meanAnomalyAtEpoch, orbit.sweep(), time)};
    const double eccAnom{orbital::findEccentricAnomaly(meanAnom, elements.eccentricity)};
    state.trueAnomaly = trueAnomaly_f(eccAnom, elements.eccentricity);
    state.radius = radius_f(elements.semiMajorAxis,  elements.eccentricity, eccAnom);
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

void compareStateVectors(const StateVector& expected, const StateVector& actual, const std::string label =""){
  for(auto i=0; i < 3; ++i){
    const double &expectedX{expected.position.c_at(i)}, &expectedV{expected.velocity.c_at(i)};
    double positionComparitor{(0 != expectedX) ? std::fabs(1e-6 * expectedX) : 5e-6 };
    double velocityComparitor{(0 != expectedV) ? std::fabs(1e-6 * expectedV) : 5e-6};
    EXPECT_NEAR(expectedX, actual.position.c_at(i), positionComparitor) << i <<" "<<label;
    EXPECT_NEAR(expectedV, actual.velocity.c_at(i), velocityComparitor) << i <<" "<<label;
  }
}

class OrbitState_trivialOrbitTests : public ::testing::Test{
public:
  const double barymass{1.0/orbital::G}, leptomass{0.0};
  const StateVector initialState{{1,0,0},{0,1,0}};

  const Orbit orbit{initialState, barymass, leptomass};
};

TEST_F(OrbitState_trivialOrbitTests, state_at_0){

  auto orbitState = calculateOrbitState(orbit, 0.0);

  compareStateVectors(initialState, orbitState.stateVector);
}

TEST_F(OrbitState_trivialOrbitTests, state_at_quarters){
  StateVector expectedState_1{{0,1,0}, {-1,0,0}};
  auto orbitState1 = calculateOrbitState(orbit, 0.5*M_PI);
  compareStateVectors(expectedState_1, orbitState1.stateVector, "State 1");

  StateVector expectedState_2{{-1,0,0}, {0, -1, 0}};
  auto orbitState2 = calculateOrbitState(orbit, M_PI);
  compareStateVectors(expectedState_2, orbitState2.stateVector, "State 2");

  StateVector expectedState_3{{0,-1,0}, {1,0,0}};
  auto orbitState3 = calculateOrbitState(orbit, 1.5*M_PI);
  compareStateVectors(expectedState_3, orbitState3.stateVector, "State 3");

  auto orbitState4 = calculateOrbitState(orbit, 2*M_PI);
  compareStateVectors(initialState, orbitState4.stateVector, "State 4");

  auto orbitState5 = calculateOrbitState(orbit, 2.5*M_PI);
  compareStateVectors(expectedState_1, orbitState5.stateVector, "State 5");

}

TEST(OrbitState_CalculationHelpers, meanAnomaly) {
  const double M0{1.23}, sweep{.123}, time{57};
  const double expectedMeanAnomaly{8.241}, actualMeanAnomaly{meanAnomaly_f(M0, sweep, time)};

  EXPECT_NEAR(expectedMeanAnomaly, actualMeanAnomaly, 1e-6 * expectedMeanAnomaly);
}

TEST(OrbitState_CalculationHelpers, trueAnomaly){
  const double eccentricAnomaly{1.23}, eccentricity{0.55};
  const double expectedTrueAnomaly{-0.264359718102521}, actualTrueAnomaly{trueAnomaly_f(eccentricAnomaly, eccentricity)};
  EXPECT_NEAR(expectedTrueAnomaly, actualTrueAnomaly, std::fabs(1e-6*expectedTrueAnomaly));
}

TEST(OrbitState_CalculationHelpers, radius){
  const double semiMajorAxis{1123}, eccentricity{0.55}, eccentricAnomaly{-0.264359718102521};
  const double expectedRadius{526.8071617363}, actualRadius{radius_f(semiMajorAxis, eccentricity, eccentricAnomaly)};
  EXPECT_NEAR(expectedRadius, actualRadius, 1e-6*expectedRadius);
}

TEST(OrbitState_CalculationHelpers, orbitCoordinateStateVector){
  OrbitalElements elements;
  elements.semiMajorAxis = 22.7429;
  elements.eccentricity = 0.11;
  Orbit orbit{elements, 1.3e12, 7e10};
  const double eccAnom{0.966520515}, trueAnom{0.488709809}, r{20.47874669};
  StateVector expectedState{{9.6145077816, 18.0814907049, 0},{-1.8324404306, 1.2574985215, 0}};
  StateVector actualState = orbitCoordinateStateVector(orbit,r, trueAnom, eccAnom);
  compareStateVectors(expectedState, actualState);
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

  compareStateVectors(expectedState, actualState);
}

// this test is very hard to calculate by hand what even the initial state should be much less a resulting state.
TEST(OrbitState, calculateOrbitState){
  const double M=1.495e30, m=3.333853e27;
  StateVector initialState{{-333833694311, 378875701458, 0},{-11374.999, -8165.466, 0}};
  const Orbit orbit{initialState, M, m};

  OrbitState state0{calculateOrbitState(orbit, 0)};
  compareStateVectors(initialState, state0.stateVector, "state0");

  OrbitState state_period{calculateOrbitState(orbit, orbit.period())};
  compareStateVectors(initialState, state_period.stateVector, "State at period to initial vector");
  compareStateVectors(state0.stateVector, state_period.stateVector, "States at 0 and Period");
}

} // anonymous namespace
} // namespace test
} // namespace engine

#endif // BUILD_TESTS
