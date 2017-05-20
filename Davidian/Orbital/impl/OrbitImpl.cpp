//
// Created by alexs on 5/19/17.
//

#include "OrbitImpl.h"

namespace orbital{
namespace impl{

double OrbitImpl::energy(const StateVector& stateVector, double standardGravitationalParameter) {
    return 0;
}

} // namespace impl
} // namespace orbital


#ifdef BUILD_TESTS

#include <gtest/gtest.h>

namespace orbital{
namespace impl{
namespace test {
namespace {

class OrbitImpl : public ::testing::Test {

};

TEST_F(OrbitImpl, energy){
    
}

} // anonymous namespace
} // namespace test
} // namespace impl
} // namespace orbital

#endif //BUILD_TESTS