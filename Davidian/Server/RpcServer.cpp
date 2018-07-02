//
// Created by alexs on 7/1/18.
//

#include "RpcServer.h"
#include "RpcServerInterface.h"

#include "OrbitalEngine.h"

#include <grpcpp/grpcpp.h>

namespace server{

std::unique_ptr<RpcServerInterface> createDefaultRpcServer(){
  return std::make_unique<RpcServer>(std::make_unique<engine::OrbitalEngine>());
}

ServiceImpl::ServiceImpl(std::unique_ptr<engine::EngineInterface>&& engine)
    : m_engine{std::move(engine)}
{}

::grpc::Status ServiceImpl::LoadFile(::grpc::ServerContext* context, const ::Davidian::engine::LoadRequest* request,
                                     ::Davidian::engine::LoadResponse* response) {
  return Service::LoadFile(context, request, response);
}

RpcServer::RpcServer(std::unique_ptr<::engine::EngineInterface>&& engine)
    : m_service{std::make_unique<ServiceImpl>(std::move(engine))}
{}

void RpcServer::startServer(const std::string& serverAddress) {
  grpc::ServerBuilder builder;
  builder.AddListeningPort(serverAddress, grpc::InsecureServerCredentials());
  builder.RegisterService(m_service.get());
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << serverAddress << std::endl;

  server->Wait();
}

RpcServer::~RpcServer() = default;

} // namespace engine

