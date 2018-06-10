//
// Created by alex on 3/17/17.
//

#include "Orbit.h"

#include "../Engine/NewtonSolver.h"

namespace orbital {
namespace {

double standardGravitationalParameter_f(double m1, double m2) {
  return orbital::G*(m1 + m2);
}

double reducedMass_f(const double m1, const double m2){
  return 1/(1/m1 + 1/m2);
}

double energy_f(const StateVector& stateVector, const double standardGravitationalParameter){
  return stateVector.velocity.dot(stateVector.velocity)/2 - standardGravitationalParameter/stateVector.position.norm();
}

double energy_f(const double semiMajorAxis, const double standardGravitationalParamter){
  return (semiMajorAxis == 0) ? 0 : -standardGravitationalParamter/(2*semiMajorAxis);
}

CartesianVector specificAngularMomentum_f(const StateVector& stateVector, const double reducedMass){
  CartesianVector angularMomentum = stateVector.position.cross(stateVector.velocity) / reducedMass;
  return angularMomentum;
}

double eccentricity_f(const double specificOrbitalEnergy, const double standardGravitationalParameter, const CartesianVector& specificAngularMomentum){
  return sqrt(1 + (2*specificOrbitalEnergy*specificAngularMomentum.dot(specificAngularMomentum))/(std::pow(standardGravitationalParameter, 2)));
}

double semiMajorAxis_f(const double standardGravitationalParameter, const double specificOrbitalEnergy){
  return -standardGravitationalParameter/(2*specificOrbitalEnergy);
}

double period_f(double semiMajorAxis, double standardGravitationalParameter) {
  return 2*M_PI*std::sqrt(std::pow(semiMajorAxis, 3)/standardGravitationalParameter);
}

double sweep_f(double period) {
  return 2*M_PI/period;
}

double inclination_f(const CartesianVector& specificAngularMomentum){
  return std::acos(specificAngularMomentum.z()/specificAngularMomentum.norm());
}

CartesianVector vectorOfAscendingNode_f(const CartesianVector& specificAngularMomentum){
  return CartesianVector{-specificAngularMomentum.y(), specificAngularMomentum.x(), 0};
}

double longitudeOfAscendingNode_f(const CartesianVector& vectorOfAscendingNode){
  return std::atan2(vectorOfAscendingNode.y(), vectorOfAscendingNode.x());
}

CartesianVector eccentricityVector_f(const StateVector& stateVector, const CartesianVector& specificAngularMomentum, const double standardGravitationalParameter){
  return stateVector.velocity.cross(specificAngularMomentum)/standardGravitationalParameter - stateVector.position.normalizedVector();
}

double argumentOfPeriapsis_f(const CartesianVector& ascendingNodeVector, const CartesianVector& eccentricityVector){
  if(CartesianVector{0,0,0} == ascendingNodeVector){return std::atan2(eccentricityVector.y(), eccentricityVector.x());}

  double candidateValue{ std::acos(ascendingNodeVector.normalizedVector().dot(eccentricityVector.normalizedVector())) };
  return (eccentricityVector.z() > 0) ? candidateValue : 2*M_PI - candidateValue;
}

double eccentricAnomaly_f(double radius, double semiMajorAxis, double eccentricity){
  return std::acos((1-radius/semiMajorAxis)/eccentricity);
}

double meanAnomaly_f(double eccentricAnomaly, double eccentricity){
  return eccentricAnomaly + eccentricity * std::sin(eccentricAnomaly);
}

double angularMomentumMagnitude_f(double stdGravParam, double semiMajorAxis, double eccentricity){
  return std::sqrt(stdGravParam*semiMajorAxis*(1-(std::pow(eccentricity, 2))));
}

double eccentricAnomaly_f(double meanAnomaly, double eccentricity){
  auto eccAnom_f = [meanAnomaly, eccentricity](const double eccAnom){
    return eccAnom - eccentricity * std::sin(eccAnom) - meanAnomaly;
  };
  auto eccAnom_d = [eccentricity](const double eccAnom){
    return 1 - eccentricity * std::cos(eccAnom);
  };

}

} // anonymous namespace

Orbit::Orbit(const StateVector& stateVector, double barymass, double leptomass)
  : m_standardGravitationalParameter{standardGravitationalParameter_f(barymass, leptomass)}
{
  m_specificOrbitalEnergy = energy_f(stateVector, m_standardGravitationalParameter);
  m_specificAngularMomentum = specificAngularMomentum_f(stateVector, reducedMass_f(barymass, leptomass));
  CartesianVector eccVector{eccentricityVector_f(stateVector, m_specificAngularMomentum, m_standardGravitationalParameter)};
//  m_elements.eccentricity = eccentricity(m_specificOrbitalEnergy, stdGravParam, m_specificAngularMomentum);
  m_elements.eccentricity = eccVector.norm();
  m_elements.semiMajorAxis = semiMajorAxis_f(m_standardGravitationalParameter, m_specificOrbitalEnergy);
  m_period = period_f(m_elements.semiMajorAxis, m_standardGravitationalParameter);
  m_sweep = sweep_f(m_period);
  m_elements.inclination = inclination_f(m_specificAngularMomentum);
  CartesianVector ascVector{vectorOfAscendingNode_f(m_specificAngularMomentum)};
  m_elements.longitudeOfAscendingNode = longitudeOfAscendingNode_f(ascVector);
  m_elements.argumentOfPeriapsis = argumentOfPeriapsis_f(ascVector, eccVector);
  const double eccAnomaly{eccentricAnomaly_f(stateVector.position.norm(), m_elements.semiMajorAxis, m_elements.eccentricity)};
  m_elements.meanAnomalyAtEpoch = meanAnomaly_f(eccAnomaly, m_elements.eccentricity);
}

Orbit::Orbit(const OrbitalElements& elements, double barymass, double leptomass)
  : m_elements{elements}
  , m_standardGravitationalParameter{standardGravitationalParameter_f(barymass, leptomass)}
{
  m_specificOrbitalEnergy = energy_f(m_elements.semiMajorAxis, m_standardGravitationalParameter);
  m_period = period_f(m_elements.semiMajorAxis, m_standardGravitationalParameter);
  m_sweep = sweep_f(m_period);
//  m_impl->m_specificOrbitalEnergy = impl::OrbitImpl::energyFromElements(m_impl->m_elements.semiMajorAxis, stdGravParam);
//  m_impl->m_period = impl::OrbitImpl::period(m_impl->m_elements.semiMajorAxis, stdGravParam);
//  m_impl->m_sweep = impl::OrbitImpl::sweep(m_impl->m_period);
//  m_impl->initializeStateVector();
}

Orbit::~Orbit() = default;

} // namespace orbital
#ifdef BUILD_TESTS

#include <gtest/gtest.h>

namespace orbital{
namespace impl{
namespace test {
namespace {

struct OrbitImplTest : public ::testing::Test {
const StateVector simpleState{{25,0,0},{0,19,0}};
const double simpleGravParam{5};
};

TEST_F(OrbitImplTest, reducedMass){
  const double m1{40}, m2{17};
  const double expectedReducedMass = 1/(1/m1 + 1/m2);
  ASSERT_NEAR(11.929824561, expectedReducedMass, .0001 * expectedReducedMass);
  EXPECT_EQ(expectedReducedMass, reducedMass_f(m1, m2));
}

TEST_F(OrbitImplTest, standardGravitationalParameter){
  // approximately 1.5e12*G
  const double expectedParameter{100.1112};
  EXPECT_NEAR(expectedParameter, standardGravitationalParameter_f(10e11, 5e11), .001);
}

TEST_F(OrbitImplTest, energy){
  EXPECT_EQ(180.3, energy_f(simpleState, simpleGravParam));

  const double semiMajorAxis{212}, standardGravitationalParameter{123}, expectedEnergy{-0.29009434};
  EXPECT_NEAR(expectedEnergy, energy_f(semiMajorAxis, standardGravitationalParameter), std::fabs(1e-6*expectedEnergy));

  EXPECT_EQ(0, energy_f(0, standardGravitationalParameter));
}

TEST_F(OrbitImplTest, specificAngularMomentum){
  const double reducedMass{7.6};
  CartesianVector expectedSpecificAngularMomentum{0,0,62.5};
  EXPECT_EQ(expectedSpecificAngularMomentum, specificAngularMomentum_f(simpleState, reducedMass));
}

TEST_F(OrbitImplTest, eccentricity){
  // manually calculated value for the given parameters
  const double expectedEccentricity{1.752428577};
  const CartesianVector specificAngularMomentum{0,0,5};
  const double specificOrbitalEnergy{7}, standardGravitationalParameter{13};
  const double actualEccentricity{eccentricity_f(specificOrbitalEnergy, standardGravitationalParameter, specificAngularMomentum)};

  EXPECT_NEAR(expectedEccentricity, actualEccentricity, 1e-6*expectedEccentricity);
}

TEST_F(OrbitImplTest, semiMajorAxis){
  const double stdGravParam{73};
  const double specificEnergy{13.42};
  const double expectedSemiMajorAxis{-stdGravParam/(2*specificEnergy)};

  const double actualSemiMajorAxis{semiMajorAxis_f(stdGravParam, specificEnergy)};

  EXPECT_NEAR(expectedSemiMajorAxis, actualSemiMajorAxis, std::fabs(1e-6*expectedSemiMajorAxis));
}

TEST_F(OrbitImplTest, inclination){
  const CartesianVector angularMomentum{std::sqrt(3.0)/2, 0.5, std::sqrt(3)};
  const double expectedInclination{M_PI/6};

  const double actualInclination{inclination_f(angularMomentum)};

  EXPECT_NEAR(expectedInclination, actualInclination, 1e-6*expectedInclination);
}

TEST_F(OrbitImplTest, inclination_zeroInclination){
  const CartesianVector angularMomentum{0, 0, 1};
  const double expectedInclination{0};

  const double actualInclination{inclination_f(angularMomentum)};

  EXPECT_EQ(expectedInclination, actualInclination);
}

TEST_F(OrbitImplTest, vectorOfAscendingNode){
  CartesianVector angularMomentum{20.0, 30.0, 10.0};
  CartesianVector expectedVector{-30.0, 20.0, 0.0};

  EXPECT_EQ(expectedVector, vectorOfAscendingNode_f(angularMomentum));
}

TEST_F(OrbitImplTest, vectorOfAscendingNode_zeroInclination){
  CartesianVector angularMomentum{0.0, 0.0, 10.0};
  CartesianVector expectedVector{0.0, 0.0, 0.0};

  EXPECT_EQ(expectedVector, vectorOfAscendingNode_f(angularMomentum));
}

TEST_F(OrbitImplTest, longitudeOfAscendingNode){
  const double baseX{std::sqrt(3.0)/2}, baseY{0.5};
  // array: x, y, expected longitude
  std::array<std::array<double, 3>, 4> testData{{
      {baseX, baseY, 2*M_PI/3},
      {-baseX, baseY, -2*M_PI/3},
      {-baseX, -baseY, -M_PI/3},
      {baseX, -baseY, M_PI/3}
  }};

  for(const auto& data : testData){
    const CartesianVector vectorOfAscendingNode{-data.at(1), data.at(0), 0};

    const double actualLongitude{longitudeOfAscendingNode_f(vectorOfAscendingNode)};

    EXPECT_NEAR(data.at(2), actualLongitude, 1e-7) << data.at(0) <<" " << data.at(1) ;
  }
}

TEST_F(OrbitImplTest, longitudeOfAscendingNode_zeroInclination){
  const CartesianVector vectorOfAscendingNode{0,0,0};
  const double expectedLongitude{0.0};
  EXPECT_EQ(expectedLongitude, longitudeOfAscendingNode_f(vectorOfAscendingNode));
}

TEST_F(OrbitImplTest, eccentricityVector){
  const CartesianVector expectedEccentricityVector{7,0,0};
  const StateVector initialState{{3,0,0},{0,4,0}};
  const CartesianVector specificAngularMomentum{0,0,12};
  const double stdGravParam{6.0};

  EXPECT_EQ(expectedEccentricityVector, eccentricityVector_f(initialState, specificAngularMomentum, stdGravParam));
}

TEST_F(OrbitImplTest, argumentOfPeriapsis){
  const CartesianVector eccVector{2,3,6}, ascVector{3,4,0};
  const double expectedValue{1.030621756};

  EXPECT_NEAR(expectedValue, argumentOfPeriapsis_f(ascVector, eccVector), 1e-6*expectedValue);
}

TEST_F(OrbitImplTest, argumentOfPeriapsis_zeroInclination){
  const CartesianVector eccVector{2,3,6}, ascVector{0,0,0};
  const double expectedValue{std::atan2(3,2)};

  EXPECT_NEAR(expectedValue, argumentOfPeriapsis_f(ascVector, eccVector), 1e-6*expectedValue);
}

TEST_F(OrbitImplTest, argumentOfPeriapsis_negativeEccentricityZ){
  const CartesianVector eccVector{2,3,-6}, ascVector{3,4,0};
  const double expectedValue{2*M_PI - 1.030621756};

  EXPECT_NEAR(expectedValue, argumentOfPeriapsis_f(ascVector, eccVector), 1e-6*expectedValue);
}

TEST_F(OrbitImplTest, eccentricAnomaly){
  const double radius{11}, semiMajorAxis{12}, eccentricity{0.25};
  const double expectedAnomaly{1.230959417};

  EXPECT_NEAR(expectedAnomaly, eccentricAnomaly_f(radius, semiMajorAxis, eccentricity), 1e-6*expectedAnomaly);
}

TEST_F(OrbitImplTest, meanAnomaly){
  const double eccentricAnomaly{1.5}, eccentricity{0.25};
  const double expectedAnomaly{1.7493737467};

  EXPECT_NEAR(expectedAnomaly, meanAnomaly_f(eccentricAnomaly, eccentricity), 1e-6*expectedAnomaly);
}

TEST_F(OrbitImplTest, period){
  const double semiMajorAxis{14}, stdGravParam{4}, expectedPeriod{164.56668702};

  EXPECT_NEAR(expectedPeriod, period_f(semiMajorAxis, stdGravParam), 1e-6*expectedPeriod);
}

TEST_F(OrbitImplTest, sweep){
  const double period{2*M_PI*11.5};
  const double expectedSweep{1/11.5};

  EXPECT_NEAR(expectedSweep, sweep_f(period), 1e-6*expectedSweep);
}

TEST_F(OrbitImplTest, angularMomentumMagnitude){
  const double semiMajorAxis{15}, stdGravParam{34}, eccentricity{0.25};
  const double expectedMagnitude{21.8660696};

  EXPECT_NEAR(expectedMagnitude, angularMomentumMagnitude_f(stdGravParam, semiMajorAxis, eccentricity), std::fabs(1e-6*expectedMagnitude));
}

} // anonymous namespace
} // namespace test
} // namespace impl
} // namespace orbital
#endif // BUILD_TESTS
