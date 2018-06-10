//
// Created by alex on 4/7/17.
//

#include "Body.h"
#include "Orbit.h"
#include "CelestialBody.h"
#include "GlobalDefinitions.h"

namespace orbital{
Body::Body(const double& mass, CelestialBody* parent, const StateVector* /*initialState*/, const double& /*time*/)
: m_mass{mass}, _parent{parent}, m_orbit{nullptr}
{
}

const double Body::standardGravitationalParameter() const {
  double totalMass = m_mass + ((nullptr != _parent) ? _parent->mass() : 0.0);
  return totalMass*orbital::G;
}

const double Body::mass() const {
  return m_mass;
}

} // namespace orbital

#ifdef BUILD_TESTS

#include <gtest/gtest.h>

namespace {

class BodyTest : public ::testing::Test{
public:
  // functionally Kerbol and Moho (to get non-trivial orbital elements)
  const double expectedChildMass{1.7565459e28}, expectedParentMass{2.5263314e21};
  const orbital::StateVector childInitialState{
    {4187626758.0595403, 461487026.3389516, 793427.7222497204},
    {-1747.2243496252022, 18184.101037188866, 38.84502337524427}
  };
  orbital::CelestialBody parentBody{expectedParentMass}, childBody{expectedChildMass, &parentBody, &childInitialState};
};

TEST_F(BodyTest, mass){
  EXPECT_EQ(expectedParentMass, parentBody.mass());
  EXPECT_EQ(expectedChildMass, childBody.mass());
}

TEST_F(BodyTest, standardGravitationalParameter){
  EXPECT_EQ(expectedParentMass*orbital::G, parentBody.standardGravitationalParameter());
  EXPECT_EQ((expectedParentMass + expectedChildMass)*orbital::G, childBody.standardGravitationalParameter());
}

TEST_F(BodyTest, standardGravitationalParameter_doesNotGrabGrandparentsMass){
  const double grandChildMass{1e18};
  const orbital::StateVector grandChildInitialState{
      {1250000, 0, 0},
      {367.30224838563566, 0, 0}
  };
  orbital::Body grandchildBody{grandChildMass, &childBody, &grandChildInitialState};

  EXPECT_EQ((expectedChildMass + grandChildMass)*orbital::G, grandchildBody.standardGravitationalParameter());
}

} // anonymous namespace for testing

#endif // BUILD_TESTS
