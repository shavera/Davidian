//
// Created by alexs on 6/20/18.
//

#include "CelestialSystemFileManager.h"

using Davidian::engine::System;

namespace engine {

System CelestialSystemFileManager::loadSystem(const std::string &filename) const {
    return Davidian::engine::System();
}

void CelestialSystemFileManager::saveSystem(const System &system, const std::string &filename) const {

}

} // namespace engine

#ifdef BUILD_TESTS

#include <gtest/gtest.h>
#include <google/protobuf/util/message_differencer.h>

#include <fstream>

namespace engine{
namespace {

using namespace testing;

System createTestSystem(){
  System system;

  auto* rootBody = system.add_body();
  rootBody->set_mass(12340);
  rootBody->set_name("Root Body");

  auto* childBody = system.add_body();
  childBody->set_mass(567);
  childBody->set_name("Child");
  childBody->set_parent_body_name("Root Body");
  auto* childOrbit = childBody->mutable_orbit();
  childOrbit->set_semimajor_axis(12.34);
  childOrbit->set_eccentricity(0.6);
  childOrbit->set_longitude_asc_node(1.75);
  childOrbit->set_inclination(0.12);
  childOrbit->set_arg_periapsis(0.45);
  childOrbit->set_mean_anomaly_0(2.33);

  return system;
};

class CSFileManagerTest : public Test{
public:

  void SetUp() override{
    remove(filePath.c_str());
    protoDifferencer.ReportDifferencesToString(&differenceString);
  }

  void TearDown() override{
    remove(filePath.c_str());
  }

  /// @note could consider using the c++17 temp file directory stuff here, but that would require bumping up to gcc-8
  const std::string filePath{"/tmp/CelestialSystemTest.pbuf"};

  void writeDefaultToTestFile(){
    std::ofstream outStream{filePath.c_str()};
    outStream << kTestSystem.SerializeAsString();
    outStream.close();
  }

  System readSystemFromTestFile(){
    std::ifstream inStream{filePath.c_str()};
    System system;
    system.ParseFromIstream(&inStream);
    inStream.close();
    return system;
  }

  const System kTestSystem{createTestSystem()};

  CelestialSystemFileManager csFileManager;

  google::protobuf::util::MessageDifferencer protoDifferencer;
  std::string differenceString;
};

TEST_F(CSFileManagerTest, loadSystem){
  writeDefaultToTestFile();

  auto system = csFileManager.loadSystem(filePath);

  EXPECT_TRUE(protoDifferencer.Compare(kTestSystem, system)) << differenceString;
}

TEST_F(CSFileManagerTest, loadSystem_fileNotPresent){
  auto system = csFileManager.loadSystem(filePath);

  EXPECT_TRUE(protoDifferencer.Compare(System{}, system)) << differenceString;
}

TEST_F(CSFileManagerTest, saveSystem){
  csFileManager.saveSystem(kTestSystem, filePath);

  auto readInSystem = readSystemFromTestFile();

  EXPECT_TRUE(protoDifferencer.Compare(kTestSystem, readInSystem)) << differenceString;
}

} // anonymous namespace
} // namespace engine

#endif // BUILD_TESTS