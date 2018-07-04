//
// Created by alexs on 7/1/18.
//

#ifndef DAVIDIAN_RPCSERVER_H
#define DAVIDIAN_RPCSERVER_H

#include "RpcServerInterface.h"

#include <Engine.grpc.pb.h>

namespace engine{
class EngineInterface;
}

namespace server {

class ServiceImpl : public Davidian::engine::Server::Service{
public:
  ServiceImpl(std::unique_ptr<engine::EngineInterface>&& engine);

  ::grpc::Status LoadSystem(::grpc::ServerContext*,
                          const ::Davidian::engine::LoadRequest* request,
                          ::Davidian::engine::System* response) override;

  ::grpc::Status GetCurrentSystem(::grpc::ServerContext* context,
                                  const ::Davidian::engine::GetCurrentSystemRequest* request,
                                  ::Davidian::engine::System* response) override;

  ::grpc::Status GetBodyStateAtTime(::grpc::ServerContext* context,
                                     const ::Davidian::engine::BodyStateRequest* request,
                                    ::Davidian::engine::OrbitState* response) override;

  ::grpc::Status GetBodyStream(::grpc::ServerContext* context,
                               const ::Davidian::engine::BodyStateStreamRequest* request,
                               ::grpc::ServerWriter<::Davidian::engine::OrbitState>* writer) override;

private:
  std::unique_ptr<engine::EngineInterface> m_engine;
};

class RpcServer final : public RpcServerInterface{
public:
  RpcServer(std::unique_ptr<engine::EngineInterface>&& engine);
  virtual ~RpcServer();

  void startServer(const std::string& serverAddress) override;

private:
  std::unique_ptr<ServiceImpl> m_service;
};

} // namespace server


#endif //DAVIDIAN_RPCSERVER_H
