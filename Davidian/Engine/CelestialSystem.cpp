//
// Created by alexs on 6/24/18.
//

#include "CelestialSystem.h"
#include "OrbitalHistory.h"

#include "Body.h"
#include "CelestialBody.h"
#include "Orbit.h"

#include <Orbital.pb.h>
#include <Engine.pb.h>

using orbital::Body;
using orbital::CelestialBody;

namespace engine {

namespace {

using System_proto = Davidian::engine::System;
using BodiesMap = std::unordered_map<std::string, std::unique_ptr<orbital::Body>>;

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

Davidian::orbital::OrbitalElements toProto(const ::orbital::OrbitalElements& elements){
  Davidian::orbital::OrbitalElements proto;
  proto.set_semimajor_axis(elements.semiMajorAxis);
  proto.set_eccentricity(elements.eccentricity);
  proto.set_inclination(elements.inclination);
  proto.set_longitude_asc_node(elements.longitudeOfAscendingNode);
  proto.set_arg_periapsis(elements.argumentOfPeriapsis);
  proto.set_mean_anomaly_0(elements.meanAnomalyAtEpoch);
  return proto;
}

struct CelestialSystemConstructionHelper{
  CelestialSystemConstructionHelper(const System_proto& proto, BodiesMap& bodiesRef)
  : systemProto{proto}, m_bodiesRef{bodiesRef}{}

  std::optional<int> indexOfRootBody(const System_proto& systemProto){
    for(auto i=0; i < systemProto.body_size(); ++i){
      if(systemProto.body(i).has_root_body()){return i;}
    }
    return std::optional<int>();
  }

  std::optional<std::string> emplaceRootBody(){
    auto index = indexOfRootBody(systemProto);
    if(!index.has_value()){return {};}
    const auto& proto = systemProto.body(index.value());
    m_bodiesRef.emplace(proto.name(), std::make_unique<CelestialBody>(proto.mass()));
    return proto.name();
  }

  std::vector<int> childrenOfBody(const std::string& parentName) const {
    std::vector<int> childIndices;
    for(auto i = 0; i < systemProto.body_size(); ++i){
      const auto& body = systemProto.body(i);
      if((body.has_free_body() && body.free_body().parent_body_name() == parentName)
          || (body.has_celestial_body() && body.celestial_body().parent_body_name() == parentName)){
        childIndices.push_back(i);
      }
    }
    return childIndices;
  }

  void constructCelestialBody(const Davidian::orbital::Body& body, const CelestialBody* parent){
    const auto& orbit = body.celestial_body().orbit();
    auto managedCelestialBody = std::make_unique<CelestialBody>(body.mass(), fromProto(orbit), parent);
    auto* newCB = managedCelestialBody.get();
    m_bodiesRef.emplace(body.name(), std::move(managedCelestialBody));
    auto grandchildIndices = childrenOfBody(body.name());
    emplaceChildren(grandchildIndices, newCB);
  }

  void emplaceChildren(const std::vector<int> childIndices, const CelestialBody* parent){
    for(int i : childIndices){
      const auto& body = systemProto.body(i);
      if(body.has_celestial_body()){
        constructCelestialBody(body, parent);
      } else if(body.has_free_body()){
        const auto& orbit = body.free_body().orbit();
        m_bodiesRef.emplace(body.name(), std::make_unique<Body>(body.mass(), fromProto(orbit), parent));
      }
    }
  }

  const System_proto& systemProto;

  BodiesMap& m_bodiesRef;
};

struct BodyProtoBuildHelper{
  explicit BodyProtoBuildHelper(const BodiesMap& bodiesRef)
  : m_bodiesRef{bodiesRef}
  {}

  std::tuple<const ::orbital::Body*, Davidian::orbital::Body*> setCommonData(System_proto& proto, const BodiesMap::value_type& bodyPair) const
  {
    const auto* body = bodyPair.second.get();
    auto* bodyProto = proto.add_body();
    bodyProto->set_name(bodyPair.first);
    bodyProto->set_mass(body->mass());
    return {body, bodyProto};
  }

  Davidian::orbital::BodyData* setBodyType(const ::orbital::Body* body, Davidian::orbital::Body* bodyProto) const{
    const auto* celestialBody = dynamic_cast<const CelestialBody*>(body);
    Davidian::orbital::BodyData* nonRootBodyData{nullptr};
    if(nullptr != celestialBody){
      if(nullptr == celestialBody->orbit()){
        bodyProto->mutable_root_body();
      } else {
        nonRootBodyData = bodyProto->mutable_celestial_body();
      }
    } else {
      nonRootBodyData = bodyProto->mutable_free_body();
    }
    return nonRootBodyData;
  }

  std::string parentName(const ::orbital::Body* body) const{
    const auto* parent = body->parent();
    auto parentBodyIt = std::find_if(m_bodiesRef.cbegin(), m_bodiesRef.cend(),
                                     [parent](const auto& bodyPair){ return bodyPair.second.get() == parent; });
    if(parentBodyIt != m_bodiesRef.cend()){
      return parentBodyIt->first;
    }
    return {};
  }

  void setNonRootBodyData(Davidian::orbital::BodyData* data, const ::orbital::Body* body) const{
    data->set_parent_body_name(parentName(body));
    const auto* orbit = body->orbit();
    data->mutable_orbit()->CopyFrom(toProto(orbit->orbitalElements()));
  }

  System_proto constructSystemProto() const{
    System_proto proto;
    for(const auto& bodyPair : m_bodiesRef){
      auto [body, bodyProto] = setCommonData(proto, bodyPair);
      auto* nonRootBodyData = setBodyType(body, bodyProto);
      if(nullptr != nonRootBodyData){
        setNonRootBodyData(nonRootBodyData, body);
      }
    }
    return proto;
  }

  const BodiesMap& m_bodiesRef;
};

} // anonymous namespace

CelestialSystem::CelestialSystem(const System_proto& systemProto) {
  CelestialSystemConstructionHelper helper{systemProto, m_bodies};
  auto rootName = helper.emplaceRootBody();
  if(rootName.has_value()){
    const std::unique_ptr<orbital::Body>& managedRoot = m_bodies.at(rootName.value());
    const auto* rootBody = dynamic_cast<const CelestialBody*>(managedRoot.get());
    helper.emplaceChildren(helper.childrenOfBody(rootName.value()), rootBody);
  }
}

const orbital::Body* CelestialSystem::body(const std::string& bodyName) const{
  if(0 == m_bodies.count(bodyName)){return nullptr;}
  return m_bodies.at(bodyName).get();
}

orbital::Body* CelestialSystem::body(const std::string& bodyName) {
  if(0 == m_bodies.count(bodyName)){return nullptr;}
  return m_bodies.at(bodyName).get();
}

bool CelestialSystem::isValidSystem() const {
  return !m_bodies.empty();
}

Davidian::engine::System CelestialSystem::constructCurrentSystem() const {
  return BodyProtoBuildHelper{m_bodies}.constructSystemProto();
}

OrbitalHistoryMap CelestialSystem::calculateHistories() const {
  OrbitalHistoryMap historyMap;
  for(const auto& bodyPair : m_bodies){
    if(nullptr != bodyPair.second->orbit()){
      historyMap.emplace(bodyPair.first, std::make_unique<OrbitalHistory>(*(bodyPair.second->orbit())));
    }
  }
  return historyMap;
}

CelestialSystem::~CelestialSystem() = default;

} // namespace engine
#ifdef BUILD_TESTS

#include "UnitTestHelpers.h"

#include "Orbit.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <google/protobuf/util/message_differencer.h>

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

  auto* parent = testSystem.body("Root Body");
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

TEST(CelestialSystemTest, constructCurrentSystem){
  auto complexSystemProto = createComplicatedSystem();
  CelestialSystem system{complexSystemProto};
  auto currentSystemProto = system.constructCurrentSystem();

  google::protobuf::util::MessageDifferencer protoDifferencer;
  std::string differenceString;
  protoDifferencer.ReportDifferencesToString(&differenceString);
  auto* systemDescriptor = currentSystemProto.GetDescriptor();
  auto* bodyDescriptor = systemDescriptor->FindFieldByName("body");
  protoDifferencer.TreatAsSet(bodyDescriptor);
  EXPECT_TRUE(protoDifferencer.Compare(complexSystemProto, currentSystemProto)) << differenceString;
}

TEST(CelestialSystemTest, calculateHistories_simpleSystem){
  auto testSystem = createTestSystem();
  CelestialSystem system{testSystem};

  auto testElements = retrieveTestOrbitalElements();
  orbital::Orbit testOrbit{testElements, system.body("Root Body")->mass(), system.body("Child")->mass()};
  OrbitalHistory expectedHistory{testOrbit};
  const double testTime{0.7*testOrbit.period()};
  auto expectedState = expectedHistory.approximateStateAtTime(testTime);

  auto histories = system.calculateHistories();

  ASSERT_EQ(1u, histories.size());
  ASSERT_EQ(1u, histories.count("Child"));
  const auto* childHistory = histories.at("Child").get();
  auto actualState = childHistory->approximateStateAtTime(testTime);

  EXPECT_GT(1e-6, expectedState.position.separation(actualState.position));
  EXPECT_GT(1e-6, expectedState.velocity.separation(actualState.velocity));
}

} // anonymous namespace
} // namespace engine

#endif // BUILD_TESTS