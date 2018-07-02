//
// Created by alexs on 7/1/18.
//

#include "RpcServer.h"

#include "OrbitalEngine.h"

#include <grpcpp/grpcpp.h>

namespace server{

std::unique_ptr<RpcServerInterface> createDefaultRpcServer(){
  return std::make_unique<RpcServer>(std::make_unique<engine::OrbitalEngine>());
}

ServiceImpl::ServiceImpl(std::unique_ptr<engine::EngineInterface>&& engine)
    : m_engine{std::move(engine)}
{}

::grpc::Status ServiceImpl::LoadFile(::grpc::ServerContext*,
                                     const ::Davidian::engine::LoadRequest* request,
                                     ::Davidian::engine::LoadResponse* response) {
  m_engine->loadSystem(request->filename());
  if(m_engine->hasValidSystem()){
    response->mutable_system()->CopyFrom(m_engine->getCurrentSystem());
    return ::grpc::Status::OK;
  } else {
    return ::grpc::Status{::grpc::NOT_FOUND, "Failed to load system, maybe file not found."};
  }
//  return Service::LoadFile(nullptr, request, response);
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

