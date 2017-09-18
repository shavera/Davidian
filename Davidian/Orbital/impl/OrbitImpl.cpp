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

double energy(const StateVector& stateVector, const double standardGravitationalParameter){
    return stateVector.velocity.dot(stateVector.velocity)/2 - standardGravitationalParameter/stateVector.position.norm();
}

CartesianVector specificAngularMomentum(const StateVector& stateVector, const double reducedMass){
    CartesianVector angularMomentum = stateVector.position.cross(stateVector.velocity) / reducedMass;
}

} // anonymous namespace

    OrbitImpl::OrbitImpl(const StateVector& stateVector, const double barymass, const double leptomass)
//        :
    {}

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

TEST_F(OrbitImpl, energy){
    EXPECT_EQ(180.3, energy(simpleState, simpleGravParam));
}

TEST_F(OrbitImpl, specificAngularMomentum){
    const double reducedMass{7.6};
    CartesianVector expectedSpecificAngularMomentum{0,0,62.5};
    CartesianVector actualAngularMomentum{specificAngularMomentum(simpleState, reducedMass)};
//    EXPECT_EQ(expectedSpecificAngularMomentum, actualAngularMomentum);
//    EXPECT_EQ(expectedSpecificAngularMomentum, specificAngularMomentum(simpleState, reducedMass));
}

} // anonymous namespace
} // namespace test
} // namespace impl
} // namespace orbital

#endif //BUILD_TESTS