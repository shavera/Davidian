//
// Created by alexs on 7/2/18.
//

#include "RpcServer.h"
#include <Engine_mock.grpc.pb.h>

#include "EngineInterface.h"

#include <google/protobuf/util/message_differencer.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;

namespace server {
namespace {

class MockEngine : public ::engine::EngineInterface{
public:
  MOCK_METHOD1(loadSystem, void(const std::string&));
  MOCK_METHOD1(useSystem, void(const ::engine::System_proto&));

  MOCK_CONST_METHOD0(hasValidSystem, bool());

  MOCK_CONST_METHOD0(getCurrentSystem, ::engine::System_proto());
  MOCK_CONST_METHOD2(bodyStateAtTime, std::optional<::engine::OrbitState_proto>(const std::string&, double));
};

class RpcServerTest : public Test{
public:
  void SetUp(){
    auto managedMockEngine = std::make_unique<MockEngine>();
    mockEngine = managedMockEngine.get();
    service = std::make_unique<ServiceImpl>(std::move(managedMockEngine));

    protoDifferencer.ReportDifferencesToString(&differenceString);
  }

  MockEngine* mockEngine{nullptr};
  std::unique_ptr<ServiceImpl> service;

  google::protobuf::util::MessageDifferencer protoDifferencer;
  std::string differenceString;
};

TEST_F(RpcServerTest, LoadSystem_file){
  const std::string fileName{"RpcServerTest LoadFile filename"};
  Davidian::engine::LoadRequest request;
  request.set_filename(fileName);

  engine::System_proto systemProto, actualSystemProto;
  systemProto.add_body()->set_name("TestBodyName");

  std::string actualFileName;
  EXPECT_CALL(*mockEngine, loadSystem(_))
      .Times(1).WillOnce(SaveArg<0>(&actualFileName));
  EXPECT_CALL(*mockEngine, hasValidSystem())
      .Times(1).WillOnce(Return(true));
  EXPECT_CALL(*mockEngine, getCurrentSystem())
      .Times(1).WillOnce(Return(systemProto));

  auto status = service->LoadSystem(nullptr, &request, &actualSystemProto);

  EXPECT_TRUE(status.ok());
  EXPECT_EQ(fileName, actualFileName);
  EXPECT_TRUE(protoDifferencer.Compare(systemProto, actualSystemProto)) << differenceString;
}

TEST_F(RpcServerTest, LoadSystem_useSystem){
  // Since the engine _may_ modify the system it's been provided, use a modified system in response.
  engine::System_proto systemProto, requestedSystemProto, responseProto, actualResponseProto;
  systemProto.add_body()->set_name("TestBodyName");
  responseProto.add_body()->set_name("ResponseSystemBody");
  Davidian::engine::LoadRequest request;
  request.mutable_system()->CopyFrom(systemProto);

  EXPECT_CALL(*mockEngine, useSystem(_))
              .Times(1).WillOnce(SaveArg<0>(&requestedSystemProto));
  EXPECT_CALL(*mockEngine, hasValidSystem())
              .Times(1).WillOnce(Return(true));
  EXPECT_CALL(*mockEngine, getCurrentSystem())
              .Times(1).WillOnce(Return(responseProto));

  auto status = service->LoadSystem(nullptr, &request, &actualResponseProto);

  EXPECT_TRUE(status.ok());
  EXPECT_TRUE(protoDifferencer.Compare(systemProto, requestedSystemProto)) << differenceString;
  EXPECT_TRUE(protoDifferencer.Compare(responseProto, actualResponseProto)) << differenceString;
}

TEST_F(RpcServerTest, LoadSystem_invalidSystem){
  const std::string fileName{"RpcServerTest LoadFile filename"};
  Davidian::engine::LoadRequest request;
  request.set_filename(fileName);

  engine::System_proto systemProto, actualSystemProto;
  systemProto.add_body()->set_name("TestBodyName");

  std::string actualFileName;
  EXPECT_CALL(*mockEngine, loadSystem(_))
      .Times(1).WillOnce(SaveArg<0>(&actualFileName));
  EXPECT_CALL(*mockEngine, hasValidSystem())
      .Times(1).WillOnce(Return(false));
  EXPECT_CALL(*mockEngine, getCurrentSystem()).Times(0);

  auto status = service->LoadSystem(nullptr, &request, &actualSystemProto);

  EXPECT_EQ(::grpc::NOT_FOUND, status.error_code());
  EXPECT_TRUE(protoDifferencer.Compare(Davidian::engine::System{}, actualSystemProto)) << differenceString;
}

TEST_F(RpcServerTest, GetCurrentSystem){
  Davidian::engine::GetCurrentSystemRequest request{};
  engine::System_proto systemProto, actualSystemProto;
  systemProto.add_body()->set_name("TestBodyName");

  EXPECT_CALL(*mockEngine, hasValidSystem())
              .Times(1).WillOnce(Return(true));
  EXPECT_CALL(*mockEngine, getCurrentSystem())
              .Times(1).WillOnce(Return(systemProto));

  auto status = service->GetCurrentSystem(nullptr, &request, &actualSystemProto);

  EXPECT_TRUE(status.ok());
  EXPECT_TRUE(protoDifferencer.Compare(systemProto, actualSystemProto)) << differenceString;
}

TEST_F(RpcServerTest, GetCurrentSystem_invalidSystem){
  Davidian::engine::GetCurrentSystemRequest request{};
  engine::System_proto systemProto, actualSystemProto;
  systemProto.add_body()->set_name("TestBodyName");

  EXPECT_CALL(*mockEngine, hasValidSystem())
      .Times(1).WillOnce(Return(false));
  EXPECT_CALL(*mockEngine, getCurrentSystem()).Times(0);

  auto status = service->GetCurrentSystem(nullptr, &request, &actualSystemProto);

  EXPECT_EQ(::grpc::FAILED_PRECONDITION, status.error_code());
  EXPECT_TRUE(protoDifferencer.Compare(Davidian::engine::System{}, actualSystemProto)) << differenceString;
}

TEST_F(RpcServerTest, BodyStateAtTime){
  const std::string name{"Test Body Name"};
  std::string requestedName;
  const double time{36.45};
  double requestedTime;
  Davidian::engine::OrbitState orbitState, returnedOrbitState;
  orbitState.set_time(26.25);

  Davidian::engine::BodyStateRequest request;
  request.set_bodyname(name);
  request.set_time(time);

  EXPECT_CALL(*mockEngine, hasValidSystem())
              .Times(1).WillOnce(Return(true));
  EXPECT_CALL(*mockEngine, bodyStateAtTime(_,_))
              .Times(1).WillOnce(DoAll(
                  SaveArg<0>(&requestedName),
                  SaveArg<1>(&requestedTime),
                  Return(orbitState)));

  auto status = service->GetBodyStateAtTime(nullptr, &request, &returnedOrbitState);

  EXPECT_TRUE(status.ok());
  EXPECT_EQ(name, requestedName);
  EXPECT_EQ(time, requestedTime);
  EXPECT_TRUE(protoDifferencer.Compare(orbitState, returnedOrbitState)) << differenceString;
}

TEST_F(RpcServerTest, BodyStateAtTime_invalidSystem){
  const std::string name{"Test Body Name"};
  Davidian::engine::BodyStateRequest request;
  request.set_bodyname(name);
  request.set_time(13.22);
  Davidian::engine::OrbitState orbitState;

  EXPECT_CALL(*mockEngine, hasValidSystem())
      .Times(1).WillOnce(Return(false));
  EXPECT_CALL(*mockEngine, bodyStateAtTime(_,_)).Times(0);

  auto status = service->GetBodyStateAtTime(nullptr, &request, &orbitState);

  EXPECT_EQ(::grpc::FAILED_PRECONDITION, status.error_code());
  EXPECT_TRUE(protoDifferencer.Compare(Davidian::engine::OrbitState{}, orbitState)) << differenceString;
}

/// @test in this case the system is valid, but the body state request fails (most likely because the body dne)
TEST_F(RpcServerTest, BodyStateAtTime_invalidRequest){
  const std::string name{"Test Body Name"};
  Davidian::engine::BodyStateRequest request;
  request.set_bodyname(name);
  request.set_time(13.22);
  Davidian::engine::OrbitState orbitState;

  EXPECT_CALL(*mockEngine, hasValidSystem())
      .Times(1).WillOnce(Return(true));
  EXPECT_CALL(*mockEngine, bodyStateAtTime(_,_))
      .Times(1)
      .WillOnce(Return(std::optional<Davidian::engine::OrbitState>{}));

  auto status = service->GetBodyStateAtTime(nullptr, &request, &orbitState);

  EXPECT_EQ(::grpc::NOT_FOUND, status.error_code());
  EXPECT_TRUE(protoDifferencer.Compare(Davidian::engine::OrbitState{}, orbitState)) << differenceString;
}

/// @note Disable the streaming tests because there isn't a way to deal with a mock stream writer
TEST_F(RpcServerTest, DISABLED_BodyStateStream){
  const std::string name{"Test Body Name"};
  std::string actualName;
  const double startTime{11.1}, stopTime{11.7}, secondsPerFrame{0.2};
  Davidian::engine::BodyStateStreamRequest request;
  request.set_bodyname(name);
  request.set_time_start(startTime);
  request.set_time_end(stopTime);
  request.set_seconds_per_frame(secondsPerFrame);

  std::vector<Davidian::engine::OrbitState> expectedStates;
  size_t expectedStateCount{static_cast<size_t>(std::fabs(stopTime - startTime) / secondsPerFrame)};
  for(size_t i = 0; i < expectedStateCount; ++i) {
    Davidian::engine::OrbitState orbitState;
    orbitState.set_time(startTime + i * secondsPerFrame);
    expectedStates.push_back(orbitState);
  }

//  Mock

}

TEST_F(RpcServerTest, DISABLED_BodyStateStream_invalidSystem){

}

TEST_F(RpcServerTest, DISABLED_BodyStateStream_invalidRequest){

}

} // anonymous namespace
} // namespace server