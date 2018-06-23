//
// Created by Alex Shaver on 10/8/17.
//

#include "OrbitalEngine.h"
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

}

System_proto OrbitalEngine::getCurrentSystem() {
  return engine::System_proto();
}

void OrbitalEngine::useSystem(const System_proto& system) {

}

bool OrbitalEngine::hasValidSystem() const {
  return false;
}

System_proto OrbitalEngine::advanceSystemToTime(const double t) {
  return engine::System_proto();
}

std::optional<StateVector_proto> OrbitalEngine::bodyStateAtTime(const std::string& bodyName, const double t) const {
  return std::optional<StateVector_proto>();
}

OrbitalEngine::~OrbitalEngine() = default;

} // namespace engine

#ifdef BUILD_TESTS

#include "UnitTestHelpers.h"

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

TEST_F(OrbitalEngineTest, useSystem){
  orbitalEngine->useSystem(kTestSystem);

  EXPECT_TRUE(orbitalEngine->hasValidSystem());
  compareProtos(kTestSystem, orbitalEngine->getCurrentSystem());
}

} // anonymous namespace
} // namespace engine

#endif // BUILD_TESTS
