//
// Created by alexs on 5/19/17.
//

#include "OrbitImpl.h"

namespace orbital{
namespace impl{

namespace {

struct EnhancedOrbitalElements{
  orbital::OrbitalElements elements;
  double energy{0.0};
};

double reducedMass(const double m1, const double m2){
  return 1/(1/m1 + 1/m2);
};

double standardGravitationalParameter(const double m1, const double m2){
  return orbital::G*(m1 + m2);
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

} // anonymous namespace

OrbitImpl::OrbitImpl(const StateVector& stateVector, const double barymass, const double leptomass)
{
  const double stdGravParam{standardGravitationalParameter(barymass, leptomass)};
  m_specificOrbitalEnergy = energy(stateVector, stdGravParam);
  m_specificAngularMomentum = specificAngularMomentum(stateVector, reducedMass(barymass, leptomass));
  m_elements.eccentricity = eccentricity(m_specificOrbitalEnergy, stdGravParam, m_specificAngularMomentum);
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
  EXPECT_NEAR(expectedParameter, standardGravitationalParameter(10e11, 5e11), .001);
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

/// Need to do another test here of the actual orbit impl, will do this later
TEST_F(OrbitImpl, DISABLED_wholeImplIntializationTest){}

} // anonymous namespace
} // namespace test
} // namespace impl
} // namespace orbital

#endif //BUILD_TESTS