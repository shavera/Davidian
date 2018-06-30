//
// Created by alexs on 6/24/18.
//

#include <Orbital.pb.h>
#include <Engine.pb.h>
#include "CelestialSystem.h"
#include "Body.h"

using orbital::Body;
using orbital::CelestialBody;

namespace engine {

namespace {

using System_proto = Davidian::engine::System;

::orbital::OrbitalElements fromProto(const Davidian::orbital::OrbitalElements& proto){
  return ::orbital::OrbitalElements{
    proto.semimajor_axis(),
    proto.eccentricity(),
    proto.inclination(),
    proto.longitude_asc_node(),
    proto.arg_periapsis(),
    proto.mean_anomaly_0()
  };
}

std::optional<int> indexOfBodyNamed(const System_proto& systemProto, const std::string& name){
  for(auto i=0; i < systemProto.body_size(); ++i){
    if(systemProto.body(i).name() == name){return i;}
  }
  return std::optional<int>();
}

std::optional<int> indexOfRootBody(const System_proto& systemProto){
  for(auto i=0; i < systemProto.body_size(); ++i){
    if(systemProto.body(i).has_root_body()){return i;}
  }
  return std::optional<int>();
}
//
//std::unique_ptr<orbital::Body> makeBody(const Davidian::orbital::Body bodyProto){
//  if()
//}

} // anonymous namespace

CelestialSystem::CelestialSystem(const System_proto& systemProto) {
  auto rootIndex = indexOfRootBody(systemProto);
  if(!rootIndex.has_value()){return;}

//  const auto& rootBodyProto = systemProto.body(rootIndex.value());
//  m_bodies.emplace(rootBodyProto.name(), std::make_unique<CelestialBody>(rootBodyProto.mass()));

  for(const auto& body : systemProto.body()){

  }
}

orbital::Body* CelestialSystem::body(const std::string& bodyName){

}

bool CelestialSystem::isValidSystem() const {
  return false;
}

CelestialSystem::~CelestialSystem() = default;

} // namespace engine
#ifdef BUILD_TESTS

#include "UnitTestHelpers.h"

#include "Orbit.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;

namespace engine{
namespace {

TEST(CelestialSystemTest, convertOrbitalElementsFromProto){
  orbital::OrbitalElements expectedElements{1'234, 0.1, 0.2, 0.3, 0.4, 0.5};
  Davidian::orbital::OrbitalElements protoElements{[](){
    Davidian::orbital::OrbitalElements proto;
    proto.set_semimajor_axis(1'234);
    proto.set_eccentricity(0.1);
    proto.set_inclination(0.2);
    proto.set_longitude_asc_node(0.3);
    proto.set_arg_periapsis(0.4);
    proto.set_mean_anomaly_0(0.5);
    return proto;
  }()
  };

  auto actualElements = fromProto(protoElements);

  EXPECT_EQ(expectedElements.semiMajorAxis, actualElements.semiMajorAxis);
  EXPECT_EQ(expectedElements.eccentricity, actualElements.eccentricity);
  EXPECT_EQ(expectedElements.inclination, actualElements.inclination);
  EXPECT_EQ(expectedElements.longitudeOfAscendingNode, actualElements.longitudeOfAscendingNode);
  EXPECT_EQ(expectedElements.argumentOfPeriapsis, actualElements.argumentOfPeriapsis);
  EXPECT_EQ(expectedElements.meanAnomalyAtEpoch, actualElements.meanAnomalyAtEpoch);
}

TEST(CelestialSystemTest, testSystem){
  auto testSystemProto = createTestSystem();
  CelestialSystem testSystem{testSystemProto};

  EXPECT_TRUE(testSystem.isValidSystem());

  auto* parent = testSystem.body("Parent");
  ASSERT_THAT(parent, NotNull());
  EXPECT_EQ(testSystemProto.body(0).mass(), parent->mass());

  auto* child = testSystem.body("Child");
  ASSERT_THAT(parent, NotNull());
  EXPECT_EQ(testSystemProto.body(1).mass(), child->mass());
  ASSERT_THAT(child->orbit(), NotNull());
  const auto& childElements = child->orbit()->orbitalElements();
  auto expectedElements = fromProto(testSystemProto.body(1).celestial_body().orbit());

  EXPECT_EQ(expectedElements.semiMajorAxis, childElements.semiMajorAxis);
  EXPECT_EQ(expectedElements.eccentricity, childElements.eccentricity);
  EXPECT_EQ(expectedElements.inclination, childElements.inclination);
  EXPECT_EQ(expectedElements.longitudeOfAscendingNode, childElements.longitudeOfAscendingNode);
  EXPECT_EQ(expectedElements.argumentOfPeriapsis, childElements.argumentOfPeriapsis);
  EXPECT_EQ(expectedElements.meanAnomalyAtEpoch, childElements.meanAnomalyAtEpoch);
}

TEST(CelestialSystemTest, invalidTestSystem){
  auto rootlessSystemProto = createRootlessSystem();
  CelestialSystem invalidSystem{rootlessSystemProto};

  EXPECT_FALSE(invalidSystem.isValidSystem());

  auto* child = invalidSystem.body("Child");
  ASSERT_THAT(child, IsNull());
}

} // anonymous namespace
} // namespace engine

#endif // BUILD_TESTS