//
// Created by alexs on 7/3/18.
//

#include "Orbit.h"

#include "GlobalDefinitions.h"

#include <gtest/gtest.h>

namespace orbital{
namespace impl{
namespace test {
namespace {
/*
 * These test helper functions in the Orbit Source file. Since they aren't publicly exposed methods, they needn't be
 * tested explicitly here. Though it may make the case for having a common file to define these helper functions and
 * some tests to run them.

struct OrbitImplTest : public ::testing::Test {
  const StateVector simpleState{{25,0,0},{0,19,0}};
  const double simpleGravParam{5};
};

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
  CartesianVector expectedSpecificAngularMomentum{0,0,475};
  EXPECT_EQ(expectedSpecificAngularMomentum, specificAngularMomentum_f(simpleState));
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

TEST_F(OrbitImplTest, eccentricityVector) {
  {
    const CartesianVector expectedEccentricityVector{7, 0, 0};
    const StateVector initialState{{3, 0, 0},
                                   {0, 4, 0}};
    const CartesianVector specificAngularMomentum{0, 0, 12};
    const double stdGravParam{6.0};

    EXPECT_EQ(expectedEccentricityVector, eccentricityVector_f(initialState, specificAngularMomentum, stdGravParam));
  }
  {
    const CartesianVector expectedEccentricityVector{0, -99, 0};
    const StateVector initialState{{0, -8, 0}, {15, 0, 0}};
    const CartesianVector specificAngularMomentum{specificAngularMomentum_f(initialState)};
    const double stdGravParam{18.0};
  }
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
  const double expectedAnomaly{1.2506262533};

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
  const double expectedMagnitudeSquared{478.125};

  EXPECT_NEAR(expectedMagnitudeSquared, angularMomentumMagnitudeSquared_f(stdGravParam, semiMajorAxis, eccentricity),
              std::fabs(1e-6*expectedMagnitudeSquared));
}

TEST_F(OrbitImplTest, trueRadius){
  const double semiMajorAxis{1e6}, eccentricity{0.12}, eccentricAnomaly{0.5235987756};
  const double expectedRadius{896076.95};

  EXPECT_NEAR(expectedRadius, radius_f(semiMajorAxis, eccentricity, eccentricAnomaly), 1e-6*expectedRadius);
}
*/

TEST(OrbitTest, fromStateVector){
  const double M=1.495e30, m=3.333853e27;
  const OrbitalElements expectedElements{
      5e11,
      0.1,
      0.0,
      0.0,
      M_PI/6,
      M_PI/2};
  StateVector initialState{{-333833694311, 378875701458, 0},{-11374.999, -8165.466, 0}};

  Orbit orbit{initialState, M, m};

  const auto& actualElements = orbit.orbitalElements();
  EXPECT_NEAR(expectedElements.semiMajorAxis, actualElements.semiMajorAxis,
              std::fabs(1e-6*expectedElements.semiMajorAxis));
  EXPECT_NEAR(expectedElements.eccentricity, actualElements.eccentricity,
              std::fabs(7.5e-5*expectedElements.eccentricity)); // about as close as I could get
  EXPECT_NEAR(expectedElements.inclination, actualElements.inclination,
              std::fabs(1e-6*expectedElements.inclination));
  EXPECT_NEAR(expectedElements.longitudeOfAscendingNode, actualElements.longitudeOfAscendingNode,
              std::fabs(1e-6*expectedElements.longitudeOfAscendingNode));
  EXPECT_NEAR(expectedElements.argumentOfPeriapsis, actualElements.argumentOfPeriapsis,
              std::fabs(1e-6*expectedElements.argumentOfPeriapsis));
  EXPECT_NEAR(expectedElements.meanAnomalyAtEpoch, actualElements.meanAnomalyAtEpoch,
              std::fabs(1e-5*expectedElements.meanAnomalyAtEpoch ));
}

TEST(OrbitTest, fromTrivialStateVector){
  const double M={1/G}, m{0};
  StateVector initalState{{1,0,0},{0,1,0}};
  const OrbitalElements expectedElements{
      1,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0};

  Orbit orbit{initalState, M, m};

  const auto& actualElements = orbit.orbitalElements();
  EXPECT_NEAR(expectedElements.semiMajorAxis, actualElements.semiMajorAxis,
              std::fabs(1e-6*expectedElements.semiMajorAxis));
  EXPECT_NEAR(expectedElements.eccentricity, actualElements.eccentricity, 1e-6); // about as close as I could get
  EXPECT_NEAR(expectedElements.inclination, actualElements.inclination, 1e-6);
  EXPECT_NEAR(expectedElements.longitudeOfAscendingNode, actualElements.longitudeOfAscendingNode, 1e-6);
  EXPECT_NEAR(expectedElements.argumentOfPeriapsis, actualElements.argumentOfPeriapsis, 1e-6);
  EXPECT_NEAR(expectedElements.meanAnomalyAtEpoch, actualElements.meanAnomalyAtEpoch, 1e-6);

}

} // anonymous namespace
} // namespace test
} // namespace impl
} // namespace orbital