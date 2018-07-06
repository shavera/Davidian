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

::grpc::Status ServiceImpl::LoadSystem(::grpc::ServerContext*,
                                     const ::Davidian::engine::LoadRequest* request,
                                     ::Davidian::engine::System* response) {
  switch(request->request_case()) {
    case Davidian::engine::LoadRequest::kFilename :
      m_engine->loadSystem(request->filename());
      break;
    case Davidian::engine::LoadRequest::kSystem :
      m_engine->useSystem(request->system());
      break;
    default:
      break;
  }

  if(m_engine->hasValidSystem()){
    response->CopyFrom(m_engine->getCurrentSystem());
    return ::grpc::Status::OK;
  } else {
    return ::grpc::Status{::grpc::NOT_FOUND, "Failed to load system, maybe file not found."};
  }
}

grpc::Status ServiceImpl::GetCurrentSystem(::grpc::ServerContext*,
                                           const ::Davidian::engine::GetCurrentSystemRequest* request,
                                           ::Davidian::engine::System* response) {
  if(m_engine->hasValidSystem()){
    response->CopyFrom(m_engine->getCurrentSystem());
    return ::grpc::Status::OK;
  } else {
    return ::grpc::Status{::grpc::FAILED_PRECONDITION, "Current system is not valid. Try to load again."};
  }
}

grpc::Status ServiceImpl::GetBodyStateAtTime(::grpc::ServerContext*,
                                             const ::Davidian::engine::BodyStateRequest* request,
                                             ::Davidian::engine::OrbitState* response) {
  if(m_engine->hasValidSystem()){
    auto state = m_engine->bodyStateAtTime(request->bodyname(), request->time());
    if(state.has_value()){
      response->CopyFrom(state.value());
      return ::grpc::Status::OK;
    } else {
      return ::grpc::Status{::grpc::NOT_FOUND, "System was valid, but could not get body state; "
                                               "Body may not exist in system"};
    }
  } else {
      return ::grpc::Status{::grpc::FAILED_PRECONDITION, "Current system is not valid. Try to load again."};
  }
}

grpc::Status ServiceImpl::GetBodyStream(::grpc::ServerContext*,
                                        const ::Davidian::engine::BodyStateStreamRequest* request,
                                        ::grpc::ServerWriter<::Davidian::engine::OrbitState>* writer) {
  const double timeStart{request->time_start()}, timeEnd{request->time_end()},
      timeStepSize{request->seconds_per_frame()};
  const std::string& name{request->bodyname()};
  if(timeStart >= timeEnd){
    return  ::grpc::Status{::grpc::INVALID_ARGUMENT, "Invalid start/stop times. "
                                                     "Received " + std::to_string(timeStart)
                                                     + " to " + std::to_string(timeEnd)};
  }
  auto stateCount = static_cast<size_t>((timeEnd - timeStart)/timeStepSize);
  std::vector<double> times(stateCount);
  times.at(0) = timeStart;
  std::generate(std::next(times.begin()), times.end(),
                [timeStepSize, t = timeStart]() mutable -> double{return t += timeStepSize;});
  for(const double t : times){
    auto state = m_engine->bodyStateAtTime(name, t);
    if(state.has_value()){
      writer->Write(state.value());
    }
  }
  return ::grpc::Status::OK;
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

