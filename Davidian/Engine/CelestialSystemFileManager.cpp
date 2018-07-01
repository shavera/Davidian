//
// Created by alexs on 6/20/18.
//

#include "CelestialSystemFileManager.h"
#include <google/protobuf/io/zero_copy_stream_impl.h>

using Davidian::engine::System;

namespace engine {

System CelestialSystemFileManager::loadSystem(const std::string &filename) const {
  System system;

  auto* file = std::fopen(filename.c_str(), "r");
  if(nullptr == file) {return system;}

  google::protobuf::io::FileInputStream inputStream{fileno(file)};
  system.ParseFromZeroCopyStream(&inputStream);
  inputStream.Close();
  return system;
}

void CelestialSystemFileManager::saveSystem(const System &system, const std::string &filename) const {
  auto* file = std::fopen(filename.c_str(), "w");
  if(nullptr == file) {return;}

  google::protobuf::io::FileOutputStream outputStream{fileno(file)};
  system.SerializeToZeroCopyStream(&outputStream);
  outputStream.Close();
}

CelestialSystemFileManager::~CelestialSystemFileManager() = default;

} // namespace engine

#ifdef BUILD_TESTS

#include "UnitTestHelpers.h"

#include <gtest/gtest.h>
#include <google/protobuf/util/message_differencer.h>

#include <fstream>

namespace engine{
namespace {

using namespace testing;

class CSFileManagerTest : public Test, public TestProtoInterface{
public:

  void SetUp() override{
    remove(filePath.c_str());
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
};

TEST_F(CSFileManagerTest, loadSystem){
  writeDefaultToTestFile();

  auto system = csFileManager.loadSystem(filePath);

  compareProtos(kTestSystem, system);
}

TEST_F(CSFileManagerTest, loadSystem_fileNotPresent){
  System system;
  ASSERT_NO_THROW(system = csFileManager.loadSystem(filePath));

  compareProtos(System{}, system);
}

TEST_F(CSFileManagerTest, saveSystem){
  csFileManager.saveSystem(kTestSystem, filePath);

  auto readInSystem = readSystemFromTestFile();

  compareProtos(kTestSystem, readInSystem);
}

TEST_F(CSFileManagerTest, saveSystem_saveOverOldFile){
  csFileManager.saveSystem(kTestSystem, filePath);
  System system;
  auto* body = system.add_body();
  body->set_name("New Body");

  csFileManager.saveSystem(system, filePath);

  auto readInSystem = readSystemFromTestFile();

  compareProtos(system, readInSystem);
}

} // anonymous namespace
} // namespace engine

#endif // BUILD_TESTS