//
// Created by Alex Shaver on 10/8/17.
//

#include "OrbitalEngine.h"
#include "CelestialSystem.h"
#include "CelestialSystemFileManager.h"

#include "Engine.pb.h"
#include "Orbital.pb.h"

using ProtoSystem = Davidian::engine::System;

namespace engine{

OrbitalEngine::OrbitalEngine()
  : OrbitalEngine{std::make_unique<CelestialSystemFileManager>()}
{}

OrbitalEngine::OrbitalEngine(std::unique_ptr<CelestialSystemFileManager>&& fileManager)
  : m_fileManager{std::move(fileManager)}
{}

void OrbitalEngine::loadSystem(const std::string& filename) {
  useSystem(m_fileManager->loadSystem(filename));
}

System_proto OrbitalEngine::getCurrentSystem() {
  return (nullptr == m_celestialSystem) ? System_proto{} : m_celestialSystem->constructCurrentSystem();
}

void OrbitalEngine::useSystem(const System_proto& system) {
  m_celestialSystem = std::make_unique<CelestialSystem>(system);
}

bool OrbitalEngine::hasValidSystem() const {
  return (nullptr == m_celestialSystem) ? false : m_celestialSystem->isValidSystem();
}

System_proto OrbitalEngine::advanceSystemToTime(const double t) {
  return engine::System_proto();
}

std::optional<OrbitState_proto> OrbitalEngine::bodyStateAtTime(const std::string& bodyName, const double t) const {
  return std::optional<OrbitState_proto>();
}

OrbitalEngine::~OrbitalEngine() = default;

} // namespace engine

#ifdef BUILD_TESTS

#include "UnitTestHelpers.h"

#include "OrbitState.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace engine{
namespace {

using namespace ::testing;

class MockCSFileManager : public CelestialSystemFileManager{
public:
  MOCK_CONST_METHOD1(loadSystem, ProtoSystem(const std::string&));
  MOCK_CONST_METHOD2(saveSystem, void(const ProtoSystem&, const std::string&));
};

class InjectableOrbitalEngine : public OrbitalEngine{
public:
  explicit InjectableOrbitalEngine(std::unique_ptr<CelestialSystemFileManager>&& fileManager)
      : OrbitalEngine{std::move(fileManager)}
  {}
};

class OrbitalEngineTest : public Test, public TestProtoInterface{
public:
  OrbitalEngineTest(){
    auto managedFileManager = std::make_unique<MockCSFileManager>();
    mockFileManager = managedFileManager.get();

    orbitalEngine = std::make_unique<InjectableOrbitalEngine>(std::move(managedFileManager));

    auto* systemDescriptor = kTestSystem.GetDescriptor();
    auto* bodyDescriptor = systemDescriptor->FindFieldByName("body");
    protoDifferencer.TreatAsSet(bodyDescriptor);
  }

  MockCSFileManager* mockFileManager{nullptr};
  std::unique_ptr<OrbitalEngine> orbitalEngine{nullptr};
  ProtoSystem kTestSystem{createTestSystem()};
};

TEST_F(OrbitalEngineTest, noSystemLoaded){
  EXPECT_FALSE(orbitalEngine->hasValidSystem());

  compareProtos(System_proto{}, orbitalEngine->getCurrentSystem());
}

TEST_F(OrbitalEngineTest, loadSystem){
  const std::string dummyFileName{"Dummy File Name"};
  std::string actualFileName;
  EXPECT_CALL(*mockFileManager, loadSystem(_))
              .WillOnce(DoAll(SaveArg<0>(&actualFileName),
                  Return(kTestSystem)));

  orbitalEngine->loadSystem(dummyFileName);

  EXPECT_EQ(dummyFileName, actualFileName);
  EXPECT_TRUE(orbitalEngine->hasValidSystem());
  compareProtos(kTestSystem, orbitalEngine->getCurrentSystem());
}

/// @test if loaded system has no root body, then the system is invalid
TEST_F(OrbitalEngineTest, loadSystem_noRootBody){
  const std::string dummyFileName{"Dummy File Name"};
  const auto rootlessSystem = createRootlessSystem();
  std::string actualFileName;
  EXPECT_CALL(*mockFileManager, loadSystem(_))
      .WillOnce(DoAll(SaveArg<0>(&actualFileName),
                      Return(rootlessSystem)));

  orbitalEngine->loadSystem(dummyFileName);

  EXPECT_EQ(dummyFileName, actualFileName);
  EXPECT_FALSE(orbitalEngine->hasValidSystem());
  compareProtos(ProtoSystem{}, orbitalEngine->getCurrentSystem());
}

TEST_F(OrbitalEngineTest, useSystem){
  orbitalEngine->useSystem(kTestSystem);

  EXPECT_TRUE(orbitalEngine->hasValidSystem());
  compareProtos(kTestSystem, orbitalEngine->getCurrentSystem());
}

/// @test if provided system has no root body, then system is invalid, don't bother constructing anything
TEST_F(OrbitalEngineTest, useSystem_noRootBody){
  const auto rootlessSystem = createRootlessSystem();

  orbitalEngine->useSystem(rootlessSystem);

  EXPECT_FALSE(orbitalEngine->hasValidSystem());
  compareProtos(ProtoSystem{}, orbitalEngine->getCurrentSystem());
}

/// @test As of issue 4, we're not yet handling body transfers, should just be simple orbit.
TEST_F(OrbitalEngineTest, advanceSystemToTime_simpleOrbit){
  orbitalEngine->useSystem(unitCircleSystem());

  orbitalEngine->advanceSystemToTime(2*M_PI/4);

  auto expectedSystem = unitCircleSystem();
  expectedSystem.mutable_body(1)->mutable_celestial_body()->mutable_orbit()->set_mean_anomaly_0(2*M_PI/4);

  compareProtos(expectedSystem, orbitalEngine->getCurrentSystem());
}

TEST_F(OrbitalEngineTest, bodyStateAtTime){
  orbitalEngine->useSystem(kTestSystem);
  auto testElements = retrieveTestOrbitalElements();
  orbital::Orbit childOrbit{testElements, kTestSystem.body(0).mass(), kTestSystem.body(1).mass()};
  const double time {(2.0/7)*childOrbit.period()};
  auto expectedOrbitState = calculateOrbitState(childOrbit, time);

  auto state = orbitalEngine->bodyStateAtTime("Child", time);
  ASSERT_TRUE(state.has_value());
  auto stateProto = state.value();

  EXPECT_EQ(time, stateProto.time());
  auto protoPosition = stateProto.state_vector().position();
  orbital::CartesianVector actualPosition{protoPosition.x(), protoPosition.y(), protoPosition.z()};
  EXPECT_GT(1e-6, expectedOrbitState.stateVector.position.separation(actualPosition));
  auto protoVelocity = stateProto.state_vector().velocity();
  orbital::CartesianVector actualVelocity{protoVelocity.x(), protoVelocity.y(), protoVelocity.z()};
  EXPECT_GT(1e-6, expectedOrbitState.stateVector.velocity.separation(actualVelocity));
}

TEST_F(OrbitalEngineTest, bodyStateAtTime_invalidSystem){
  orbitalEngine->useSystem(createRootlessSystem());

  auto state = orbitalEngine->bodyStateAtTime("Child", 0.0);

  EXPECT_FALSE(state.has_value());
}

TEST_F(OrbitalEngineTest, bodyStateAtTime_bodyNameNotFound){
  orbitalEngine->useSystem(kTestSystem);

  auto state = orbitalEngine->bodyStateAtTime("Should not be a body with this name", 0.0);

  EXPECT_FALSE(state.has_value());
}

} // anonymous namespace
} // namespace engine

#endif // BUILD_TESTS
