//
// Created by alexs on 7/2/18.
//

#include "RpcServer.h"

#include "EngineInterface.h"

#include <google/protobuf/util/message_differencer.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;

namespace server {
namespace {

TEST(ServerTest, xyasdf){
  EXPECT_TRUE(true);
}

class MockEngine : public ::engine::EngineInterface{
public:
  MOCK_METHOD1(loadSystem, void(const std::string&));
  MOCK_METHOD1(useSystem, void(const ::engine::System_proto&));

  MOCK_CONST_METHOD0(hasValidSystem, bool());

  MOCK_CONST_METHOD0(getCurrentSystem, ::engine::System_proto());
  MOCK_CONST_METHOD2(bodyStateAtTime, std::optional<::engine::OrbitState_proto>(const std::string&, double));
};

class RpcServerTest : public Test{
  void SetUp() override {
//    auto managedMockEngine = std::make_unique<MockEngine>();
//    mockEngine = managedMockEngine.get();
//    service = std::make_unique<ServiceImpl>(std::move(managedMockEngine));
  }
  MockEngine* mockEngine{nullptr};
//  std::unique_ptr<ServiceImpl> service;
  ServiceImpl service{nullptr};
};

TEST_F(RpcServerTest, xysjhd){
  EXPECT_TRUE(true);
}
//
//class RpcServerTest : public Test{
//public:
//  void SetUp() override{
//    auto managedMockEngine = std::make_unique<MockEngine>();
//    mockEngine = managedMockEngine.get();
//    service = std::make_unique<ServiceImpl>(std::move(managedMockEngine));
//
//    protoDifferencer.ReportDifferencesToString(&differenceString);
//  }
//
//  MockEngine* mockEngine{nullptr};
//  std::unique_ptr<ServiceImpl> service;
//
//  google::protobuf::util::MessageDifferencer protoDifferencer;
//  std::string differenceString;
//};
//
//TEST_F(RpcServerTest, LoadFile){
//  const std::string fileName{"RpcServerTest LoadFile filename"};
//  Davidian::engine::LoadRequest request;
//  request.set_filename(fileName);
//  Davidian::engine::LoadResponse response;
//
//  engine::System_proto systemProto;
//  systemProto.add_body()->set_name("TestBodyName");
//
//  std::string actualFileName;
//  EXPECT_CALL(*mockEngine, loadSystem(_))
//      .Times(1).WillOnce(SaveArg<0>(&actualFileName));
//  EXPECT_CALL(*mockEngine, hasValidSystem())
//      .Times(1).WillOnce(Return(true));
//  EXPECT_CALL(*mockEngine, getCurrentSystem())
//      .Times(1).WillOnce(Return(systemProto));
//
//  auto status = service->LoadFile(nullptr, &request, &response);
//
//  EXPECT_TRUE(status.ok());
//  EXPECT_EQ(fileName, actualFileName);
//  EXPECT_TRUE(protoDifferencer.Compare(systemProto, response.system())) << differenceString;
//}

} // anonymous namespace
} // namespace server
