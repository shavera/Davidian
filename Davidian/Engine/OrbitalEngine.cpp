//
// Created by Alex Shaver on 10/8/17.
//

#include "OrbitalEngine.h"
#include "CelestialSystemFileManager.h"

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

OrbitalEngine::~OrbitalEngine() = default;

} // namespace engine

#ifdef BUILD_TESTS

#include <gmock/gmock.h>
#include <google/protobuf/util/message_differencer.h>
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

class OrbitalEngineTest : public Test{
public:
  OrbitalEngineTest(){
    auto managedFileManager = std::make_unique<MockCSFileManager>();
    mockFileManager = managedFileManager.get();

    orbitalEngine = std::make_unique<InjectableOrbitalEngine>(std::move(managedFileManager));

    protoDifferencer.ReportDifferencesToString(&differenceString);
  }

  MockCSFileManager* mockFileManager{nullptr};
  std::unique_ptr<OrbitalEngine> orbitalEngine{nullptr};

  google::protobuf::util::MessageDifferencer protoDifferencer;
  std::string differenceString;
};

TEST_F(OrbitalEngineTest, noSystemLoaded){
  EXPECT_FALSE(orbitalEngine->hasValidSystem());

  EXPECT_TRUE(protoDifferencer.Compare(System_proto{}, orbitalEngine->getCurrentSystem())) << differenceString;
}

TEST_F(OrbitalEngineTest, loadSystem){

}

} // anonymous namespace
} // namespace engine

#endif // BUILD_TESTS
