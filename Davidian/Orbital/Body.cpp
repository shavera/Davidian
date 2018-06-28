//
// Created by alex on 4/7/17.
//

#include "Body.h"
#include "Orbit.h"
#include "CelestialBody.h"
#include "GlobalDefinitions.h"

namespace orbital{
//Body::Body(const double& mass, CelestialBody* parent, const StateVector* /*initialState*/, const double& /*time*/)
//: m_mass{mass}, _parent{parent}, m_orbit{nullptr}
//{
//}

Body::Body(const double& mass, const OrbitalElements& orbitalElements, CelestialBody* parent)
  : m_mass{mass}
  , _parent{parent}
  , m_orbit{std::make_unique<Orbit>(orbitalElements, parent->mass(), m_mass)}
{

}

double Body::standardGravitationalParameter() const {
  double totalMass = m_mass + ((nullptr != _parent) ? _parent->mass() : 0.0);
  return totalMass*orbital::G;
}

double Body::mass() const {
  return m_mass;
}

Body::~Body() = default;

} // namespace orbital

#ifdef BUILD_TESTS

#include <gtest/gtest.h>

namespace orbital {
namespace {

class BodyTest : public ::testing::Test {
public:
  // functionally Kerbol and Moho
  const double expectedChildMass{1.7565459e28}, expectedParentMass{2.5263314e21};
  static constexpr OrbitalElements elements{
      5'263'138'304,
      0.2,
      (70/360)*2*M_PI,
      (7/360)*2*M_PI,
      (15/360)*2*M_PI,
      3.14
  };
  orbital::CelestialBody parentBody{expectedParentMass, {}}, childBody{expectedChildMass, elements, &parentBody};
};

TEST_F(BodyTest, mass) {
  EXPECT_EQ(expectedParentMass, parentBody.mass());
  EXPECT_EQ(expectedChildMass, childBody.mass());
}

TEST_F(BodyTest, standardGravitationalParameter) {
  EXPECT_EQ(expectedParentMass * orbital::G, parentBody.standardGravitationalParameter());
  EXPECT_EQ((expectedParentMass + expectedChildMass) * orbital::G, childBody.standardGravitationalParameter());
}

TEST_F(BodyTest, standardGravitationalParameter_doesNotGrabGrandparentsMass) {
  const double grandChildMass{1e18};
  constexpr OrbitalElements grandChildOrbit{300'000, 0, 0, 0, 0, 0};
  orbital::Body grandchildBody{grandChildMass,grandChildOrbit, &childBody};

  EXPECT_EQ((expectedChildMass + grandChildMass) * orbital::G, grandchildBody.standardGravitationalParameter());
}

} // anonymous namespace for testing
} // namespace orbital

#endif // BUILD_TESTS
