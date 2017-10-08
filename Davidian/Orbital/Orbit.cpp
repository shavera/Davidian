//
// Created by alex on 3/17/17.
//

#include "Orbit.h"
#include "impl/OrbitImpl.h"

namespace orbital {

Orbit::Orbit(const StateVector& stateVector, double barymass, double leptomass, impl::OrbitImpl* impl)
    : m_impl{nullptr == impl ? new impl::OrbitImpl{stateVector, barymass, leptomass} : impl}
{

}

Orbit::~Orbit() = default;

} // namespace orbital


#ifdef BUILD_TESTS

#include <gtest/gtest.h>

namespace orbital{
namespace {

class OrbitTest : public ::testing::Test{

};

TEST_F(OrbitTest, dummy){
//    Orbit orbit(StateVector{}, 1);
}

} // anonymous namespace
} // namespace orbital

#endif // BUILD_TESTS