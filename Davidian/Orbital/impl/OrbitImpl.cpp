//
// Created by alexs on 5/19/17.
//

#include "OrbitImpl.h"

#include <iostream>

namespace orbital{
namespace impl{

namespace {

double reducedMass(const double m1, const double m2){
  return 1/(1/m1 + 1/m2);
};

double energy(const StateVector& stateVector, const double standardGravitationalParameter){
  return stateVector.velocity.dot(stateVector.velocity)/2 - standardGravitationalParameter/stateVector.position.norm();
}

CartesianVector specificAngularMomentum(const StateVector& stateVector, const double reducedMass){
  CartesianVector angularMomentum = stateVector.position.cross(stateVector.velocity) / reducedMass;
  return angularMomentum;
}

double eccentricity(const double specificOrbitalEnergy, const double standardGravitationalParameter, const CartesianVector& specificAngularMomentum){
  return sqrt(1 + (2*specificOrbitalEnergy*specificAngularMomentum.dot(specificAngularMomentum))/(std::pow(standardGravitationalParameter, 2)));
}

double semiMajorAxis(const double standardGravitationalParameter, const double specificOrbitalEnergy){
  return -standardGravitationalParameter/(2*specificOrbitalEnergy);
}

double inclination(const CartesianVector& specificAngularMomentum){
  return std::acos(specificAngularMomentum.z()/specificAngularMomentum.norm());
}

CartesianVector vectorOfAscendingNode(const CartesianVector& specificAngularMomentum){
  return CartesianVector{-specificAngularMomentum.y(), specificAngularMomentum.x(), 0};
}

double longitudeOfAscendingNode(const CartesianVector& vectorOfAscendingNode){
  return std::atan2(vectorOfAscendingNode.y(), vectorOfAscendingNode.x());
}

CartesianVector eccentricityVector(const StateVector& stateVector, const CartesianVector& specificAngularMomentum, const double standardGravitationalParameter){
  return stateVector.velocity.cross(specificAngularMomentum)/standardGravitationalParameter - stateVector.position.normalizedVector();
}

double argumentOfPeriapsis(const CartesianVector& ascendingNodeVector, const CartesianVector& eccentricityVector){
  if(CartesianVector{0,0,0} == ascendingNodeVector){return std::atan2(eccentricityVector.y(), eccentricityVector.x());}

  double candidateValue{ std::acos(ascendingNodeVector.normalizedVector().dot(eccentricityVector.normalizedVector())) };
  return (eccentricityVector.z() > 0) ? candidateValue : 2*M_PI - candidateValue;
}

double eccentricAnomaly(double radius, double semiMajorAxis, double eccentricity){
  return std::acos((1-radius/semiMajorAxis)/eccentricity);
}

double meanAnomaly(double eccentricAnomaly, double eccentricity){
  return eccentricAnomaly + eccentricity * std::sin(eccentricAnomaly);
}

} // anonymous namespace

OrbitImpl::OrbitImpl(const StateVector& stateVector, double barymass, double leptomass)
{
  const double stdGravParam{standardGravitationalParameter(barymass, leptomass)};
  m_specificOrbitalEnergy = energy(stateVector, stdGravParam);
  m_specificAngularMomentum = specificAngularMomentum(stateVector, reducedMass(barymass, leptomass));
  CartesianVector eccVector{eccentricityVector(stateVector, m_specificAngularMomentum, stdGravParam)};
//  m_elements.eccentricity = eccentricity(m_specificOrbitalEnergy, stdGravParam, m_specificAngularMomentum);
  m_elements.eccentricity = eccVector.norm();
  m_elements.semiMajorAxis = semiMajorAxis(stdGravParam, m_specificOrbitalEnergy);
  m_period = period(m_elements.semiMajorAxis, stdGravParam);
  m_sweep = sweep(m_period);
  m_elements.inclination = inclination(m_specificAngularMomentum);
  CartesianVector ascVector{vectorOfAscendingNode(m_specificAngularMomentum)};
  m_elements.longitudeOfAscendingNode = longitudeOfAscendingNode(ascVector);
  m_elements.argumentOfPeriapsis = argumentOfPeriapsis(ascVector, eccVector);
  const double eccAnomaly{eccentricAnomaly(stateVector.position.norm(), m_elements.semiMajorAxis, m_elements.eccentricity)};
  m_elements.meanAnomalyAtEpoch = meanAnomaly(eccAnomaly, m_elements.eccentricity);
}

double OrbitImpl::energyFromElements(double semiMajorAxis, double standardGravitationalParameter) {
  return 0 == semiMajorAxis ? 0 : -standardGravitationalParameter/(2*semiMajorAxis);
}

double OrbitImpl::period(double semiMajorAxis, double standardGravitationalParameter) {
  return 2*M_PI*std::sqrt(std::pow(semiMajorAxis, 3)/standardGravitationalParameter);
}

double OrbitImpl::sweep(double period) {
  return 2*M_PI/period;
}

double OrbitImpl::standardGravitationalParameter(double m1, double m2) {
  return orbital::G*(m1 + m2);
}

} // namespace impl
} // namespace orbital


#ifdef BUILD_TESTS

#include <gtest/gtest.h>

namespace orbital{
namespace impl{
namespace test {
namespace {

struct OrbitImpl : public ::testing::Test {
const StateVector simpleState{{25,0,0},{0,19,0}};
const double simpleGravParam{5};
};

TEST_F(OrbitImpl, reducedMass){
  const double m1{40}, m2{17};
  const double expectedReducedMass = 1/(1/m1 + 1/m2);
  ASSERT_NEAR(11.929824561, expectedReducedMass, .0001 * expectedReducedMass);
  EXPECT_EQ(expectedReducedMass, reducedMass(m1, m2));
}

TEST_F(OrbitImpl, standardGravitationalParameter){
  // approximately 1.5e12*G
  const double expectedParameter{100.1112};
  EXPECT_NEAR(expectedParameter, orbital::impl::OrbitImpl::standardGravitationalParameter(10e11, 5e11), .001);
}

TEST_F(OrbitImpl, energy){
  EXPECT_EQ(180.3, energy(simpleState, simpleGravParam));
}

TEST_F(OrbitImpl, specificAngularMomentum){
  const double reducedMass{7.6};
  CartesianVector expectedSpecificAngularMomentum{0,0,62.5};
  EXPECT_EQ(expectedSpecificAngularMomentum, specificAngularMomentum(simpleState, reducedMass));
}

TEST_F(OrbitImpl, eccentricity){
  // manually calculated value for the given parameters
  const double expectedEccentricity{1.752428577};
  const CartesianVector specificAngularMomentum{0,0,5};
  const double specificOrbitalEnergy{7}, standardGravitationalParameter{13};
  const double actualEccentricity{eccentricity(specificOrbitalEnergy, standardGravitationalParameter, specificAngularMomentum)};

  EXPECT_NEAR(expectedEccentricity, actualEccentricity, 1e-6*expectedEccentricity);
}

TEST_F(OrbitImpl, semiMajorAxis){
  const double stdGravParam{73};
  const double specificEnergy{13.42};
  const double expectedSemiMajorAxis{-stdGravParam/(2*specificEnergy)};

  const double actualSemiMajorAxis{semiMajorAxis(stdGravParam, specificEnergy)};

  EXPECT_NEAR(expectedSemiMajorAxis, actualSemiMajorAxis, std::fabs(1e-6*expectedSemiMajorAxis));
}

TEST_F(OrbitImpl, inclination){
  const CartesianVector angularMomentum{std::sqrt(3.0)/2, 0.5, std::sqrt(3)};
  const double expectedInclination{M_PI/6};

  const double actualInclination{inclination(angularMomentum)};

  EXPECT_NEAR(expectedInclination, actualInclination, 1e-6*expectedInclination);
}

TEST_F(OrbitImpl, inclination_zeroInclination){
  const CartesianVector angularMomentum{0, 0, 1};
  const double expectedInclination{0};

  const double actualInclination{inclination(angularMomentum)};

  EXPECT_EQ(expectedInclination, actualInclination);
}

TEST_F(OrbitImpl, vectorOfAscendingNode){
  CartesianVector angularMomentum{20.0, 30.0, 10.0};
  CartesianVector expectedVector{-30.0, 20.0, 0.0};

  EXPECT_EQ(expectedVector, vectorOfAscendingNode(angularMomentum));
}

TEST_F(OrbitImpl, vectorOfAscendingNode_zeroInclination){
  CartesianVector angularMomentum{0.0, 0.0, 10.0};
  CartesianVector expectedVector{0.0, 0.0, 0.0};

  EXPECT_EQ(expectedVector, vectorOfAscendingNode(angularMomentum));
}

TEST_F(OrbitImpl, longitudeOfAscendingNode){
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

    const double actualLongitude{longitudeOfAscendingNode(vectorOfAscendingNode)};

    EXPECT_NEAR(data.at(2), actualLongitude, 1e-7) << data.at(0) <<" " << data.at(1) ;
  }
}

TEST_F(OrbitImpl, longitudeOfAscendingNode_zeroInclination){
  const CartesianVector vectorOfAscendingNode{0,0,0};
  const double expectedLongitude{0.0};
  EXPECT_EQ(expectedLongitude, longitudeOfAscendingNode(vectorOfAscendingNode));
}

TEST_F(OrbitImpl, eccentricityVector){
  const CartesianVector expectedEccentricityVector{7,0,0};
  const StateVector initialState{{3,0,0},{0,4,0}};
  const CartesianVector specificAngularMomentum{0,0,12};
  const double stdGravParam{6.0};

  EXPECT_EQ(expectedEccentricityVector, eccentricityVector(initialState, specificAngularMomentum, stdGravParam));
}

TEST_F(OrbitImpl, argumentOfPeriapsis){
  const CartesianVector eccVector{2,3,6}, ascVector{3,4,0};
  const double expectedValue{1.030621756};

  EXPECT_NEAR(expectedValue, argumentOfPeriapsis(ascVector, eccVector), 1e-6*expectedValue);
}

TEST_F(OrbitImpl, argumentOfPeriapsis_zeroInclination){
  const CartesianVector eccVector{2,3,6}, ascVector{0,0,0};
  const double expectedValue{std::atan2(3,2)};

  EXPECT_NEAR(expectedValue, argumentOfPeriapsis(ascVector, eccVector), 1e-6*expectedValue);
}

TEST_F(OrbitImpl, argumentOfPeriapsis_negativeEccentricityZ){
  const CartesianVector eccVector{2,3,-6}, ascVector{3,4,0};
  const double expectedValue{2*M_PI - 1.030621756};

  EXPECT_NEAR(expectedValue, argumentOfPeriapsis(ascVector, eccVector), 1e-6*expectedValue);
}

TEST_F(OrbitImpl, eccentricAnomaly){
  const double radius{11}, semiMajorAxis{12}, eccentricity{0.25};
  const double expectedAnomaly{1.230959417};

  EXPECT_NEAR(expectedAnomaly, eccentricAnomaly(radius, semiMajorAxis, eccentricity), 1e-6*expectedAnomaly);
}

TEST_F(OrbitImpl, meanAnomaly){
  const double eccentricAnomaly{1.5}, eccentricity{0.25};
  const double expectedAnomaly{1.7493737467};

  EXPECT_NEAR(expectedAnomaly, meanAnomaly(eccentricAnomaly, eccentricity), 1e-6*expectedAnomaly);
}

TEST_F(OrbitImpl, period){
  const double semiMajorAxis{14}, stdGravParam{4}, expectedPeriod{164.56668702};

  EXPECT_NEAR(expectedPeriod, orbital::impl::OrbitImpl::period(semiMajorAxis, stdGravParam), 1e-6*expectedPeriod);
}

TEST_F(OrbitImpl, sweep){
  const double period{2*M_PI*11.5};
  const double expectedSweep{1/11.5};

  EXPECT_NEAR(expectedSweep, orbital::impl::OrbitImpl::sweep(period), 1e-6*expectedSweep);
}

TEST_F(OrbitImpl, energyFromElements){
  const double semimMajorAxis{124}, stdGravParam{93};
  const double expectedEnergy{-0.375};

  EXPECT_EQ(expectedEnergy, orbital::impl::OrbitImpl::energyFromElements(semimMajorAxis, stdGravParam));
}


TEST_F(OrbitImpl, energyFromElements_zeroSemiMajorAxis){
  const double stdGravParam{93}, semiMajorAxis{0};

  EXPECT_EQ(0, orbital::impl::OrbitImpl::energyFromElements(semiMajorAxis, stdGravParam));
}

TEST_F(OrbitImpl, periodFromElements){

}

TEST_F(OrbitImpl, sweepFromElements){

}

/// Need to do another test here of the actual orbit impl, will do this later
TEST_F(OrbitImpl, DISABLED_wholeImplIntializationTest){}

} // anonymous namespace
} // namespace test
} // namespace impl
} // namespace orbital

#endif //BUILD_TESTS