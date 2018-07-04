//
// Created by alexs on 7/2/18.
//

#include "RpcServer.h"

#include "EngineInterface.h"

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
    server = std::make_unique<RpcServer>(std::move(managedMockEngine));
  }

  MockEngine* mockEngine{nullptr};
  std::unique_ptr<RpcServer> server;
};

TEST_F(RpcServerTest, Dummy){
  ASSERT_THAT(server, NotNull());
}

} // anonymous namespace
} // namespace server