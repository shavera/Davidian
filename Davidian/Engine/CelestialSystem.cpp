//
// Created by alexs on 6/24/18.
//

#include "CelestialSystem.h"
#include "Body.h"

namespace engine {

CelestialSystem::CelestialSystem(const System_proto& systemProto) {

}

orbital::Body* CelestialSystem::body(const std::string& bodyName){

}

CelestialSystem::~CelestialSystem() = default;

} // namespace engine
#ifdef BUILD_TESTS

#include "UnitTestHelpers.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;

namespace engine{
namespace {

TEST(CelestialSystemTest, testSystem){
  auto testSystemProto = createTestSystem();
  CelestialSystem testSystem{testSystemProto};

  auto* parent = testSystem.body("Parent");
  ASSERT_THAT(parent, NotNull());
  EXPECT_EQ(testSystemProto.body(0).mass(), parent->mass());

//  auto* child =
}

} // anonymous namespace
} // namespace engine

#endif // BUILD_TESTS