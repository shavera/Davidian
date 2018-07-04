//
// Created by alexs on 7/3/18.
//

#include "Body.h"

#include "CelestialBody.h"
#include "GlobalDefinitions.h"
#include "Orbit.h"

#include <gtest/gtest.h>

namespace orbital {
namespace {

class BodyTest : public ::testing::Test {
public:
  // functionally Kerbol and Moho
  const double expectedParentMass{1.7565459e28}, expectedChildMass{2.5263314e21};
  static constexpr OrbitalElements elements{
      5'263'138'304,
      0.2,
      (70.0/360)*2*M_PI,
      (7.0/360)*2*M_PI,
      (15.0/360)*2*M_PI,
      3.14
  };

  CelestialBody parentBody{expectedParentMass}, childBody{expectedChildMass, elements, &parentBody};
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

TEST_F(BodyTest, setParentAndOrbit){
  const double newParentMass{1.5*expectedChildMass};
  constexpr OrbitalElements newOrbit{500'000, 0.1, 0.2, 0.3, 0.4, 0.5};
  CelestialBody newParentBody{newParentMass};

  childBody.setParentAndOrbit(&newParentBody, newOrbit);

  EXPECT_NEAR(2.5*expectedChildMass*orbital::G, childBody.standardGravitationalParameter(),
              std::fabs(1e-8*2.5*expectedChildMass*orbital::G));

  ASSERT_NE(nullptr, childBody.orbit());
  auto* actualOrbit = childBody.orbit();
  // for example
  EXPECT_EQ(newOrbit.semiMajorAxis, actualOrbit->orbitalElements().semiMajorAxis);
}

TEST_F(BodyTest, rootBodyOrbit_Null){
  EXPECT_EQ(nullptr, parentBody.orbit());
}

} // anonymous namespace for testing
} // namespace orbital