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

} // namespace engine

#ifdef BUILD_TESTS

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
  InjectableOrbitalEngine(std::unique_ptr<CelestialSystemFileManager>&& fileManager)
      : OrbitalEngine{std::move(fileManager)}
  {}
};

class OrbitalEngineTest : public Test{
public:
  OrbitalEngineTest(){
    auto managedFileManager = std::make_unique<MockCSFileManager>();
    mockFileManager = managedFileManager.get();

    orbitalEngine = std::make_unique<InjectableOrbitalEngine>(std::move(managedFileManager));
  }

  MockCSFileManager* mockFileManager{nullptr};
  std::unique_ptr<OrbitalEngine> orbitalEngine{nullptr};
};

} // anonymous namespace
} // namespace engine

#endif // BUILD_TESTS
