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

const orbital::Body* CelestialSystem::body(const std::string& bodyName) const{
  return nullptr;
}

orbital::Body* CelestialSystem::body(const std::string& bodyName) {
  return nullptr;
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

int indexOfBodyNamed(const System_proto& systemProto, const std::string& name){
  for(auto i=0; i < systemProto.body_size(); ++i){
    if(systemProto.body(i).name() == name){return i;}
  }
  return -1;
}

void testMainBody(const CelestialSystem& system, const System_proto& systemProto, const std::string& name){
  SCOPED_TRACE(name);
  int index{indexOfBodyNamed(systemProto, name)};
  const auto& proto = systemProto.body(index);
  ASSERT_EQ(name, proto.name());
  auto* body = system.body(name);
  ASSERT_THAT(body, NotNull());
  EXPECT_NEAR(proto.mass(), body->mass(), 1e-6 * proto.mass());
}

void testElements(const Davidian::orbital::OrbitalElements& protoOrbit, const ::orbital::OrbitalElements& elements){
  EXPECT_NEAR(protoOrbit.semimajor_axis(), elements.semiMajorAxis,
              std::fabs(1e-6*protoOrbit.semimajor_axis()));

  EXPECT_NEAR(protoOrbit.eccentricity(), elements.eccentricity,
              std::fabs(1e-6*protoOrbit.eccentricity()));

  EXPECT_NEAR(protoOrbit.inclination(), elements.inclination,
              std::fabs(1e-6*protoOrbit.inclination()));

  EXPECT_NEAR(protoOrbit.longitude_asc_node(), elements.longitudeOfAscendingNode,
              std::fabs(1e-6*protoOrbit.longitude_asc_node()));

  EXPECT_NEAR(protoOrbit.arg_periapsis(), elements.argumentOfPeriapsis,
              std::fabs(1e-6*protoOrbit.arg_periapsis()));

  EXPECT_NEAR(protoOrbit.mean_anomaly_0(), elements.meanAnomalyAtEpoch,
              std::fabs(1e-6*protoOrbit.mean_anomaly_0()));
}

void testCelestialBody(const CelestialSystem& system, const System_proto& systemProto, const std::string& name){
  SCOPED_TRACE("CB Test" + name);
  ASSERT_NO_FATAL_FAILURE(testMainBody(system, systemProto, name));
  const auto& proto = systemProto.body(indexOfBodyNamed(systemProto, name));
  ASSERT_TRUE(proto.has_celestial_body());
  const auto& protoData = proto.celestial_body();
  const auto& protoOrbit = protoData.orbit();
  auto* body = system.body(name);
  const auto& elements = body->orbit()->orbitalElements();
  EXPECT_NO_FATAL_FAILURE(testElements(protoOrbit, elements));
}

void testFreeBody(const CelestialSystem& system, const System_proto& systemProto, const std::string& name){
  SCOPED_TRACE("FB Test" + name);
  ASSERT_NO_FATAL_FAILURE(testMainBody(system, systemProto, name));
  const auto& proto = systemProto.body(indexOfBodyNamed(systemProto, name));
  ASSERT_TRUE(proto.has_free_body());
  const auto& protoData = proto.free_body();
  const auto& protoOrbit = protoData.orbit();
  auto* body = system.body(name);
  const auto& elements = body->orbit()->orbitalElements();
  EXPECT_NO_FATAL_FAILURE(testElements(protoOrbit, elements));
}

TEST(CelestialSystemTest, complexTestSystem){
  auto complexSystemProto = createComplicatedSystem();
  CelestialSystem system{complexSystemProto};

  EXPECT_TRUE(system.isValidSystem());

  EXPECT_NO_FATAL_FAILURE(testMainBody(system, complexSystemProto, "Kerbol"));
  EXPECT_NO_FATAL_FAILURE(testCelestialBody(system, complexSystemProto, "Kerbin"));
  EXPECT_NO_FATAL_FAILURE(testCelestialBody(system, complexSystemProto, "Mun"));
  EXPECT_NO_FATAL_FAILURE(testCelestialBody(system, complexSystemProto, "Minmus"));
  EXPECT_NO_FATAL_FAILURE(testCelestialBody(system, complexSystemProto, "Duna"));
  EXPECT_NO_FATAL_FAILURE(testCelestialBody(system, complexSystemProto, "Ike"));
  EXPECT_NO_FATAL_FAILURE(testFreeBody(system, complexSystemProto, "Ship"));
}

} // anonymous namespace
} // namespace engine

#endif // BUILD_TESTS