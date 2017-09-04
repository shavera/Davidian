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
    double energy(const StateVector& stateVector, const double standardGravitationalParameter){
        return stateVector.velocity.dot(stateVector.velocity)/2 - standardGravitationalParameter/stateVector.position.norm();
    }
} // anonymous namespace

    OrbitImpl::OrbitImpl(const StateVector& stateVector, const double standardGravitationalParameter)
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

TEST_F(OrbitImpl, energy){
    EXPECT_EQ(180.3, energy(simpleState, simpleGravParam));
}

} // anonymous namespace
} // namespace test
} // namespace impl
} // namespace orbital

#endif //BUILD_TESTS