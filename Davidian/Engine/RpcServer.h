//
// Created by alexs on 7/1/18.
//

#ifndef DAVIDIAN_RPCSERVER_H
#define DAVIDIAN_RPCSERVER_H

#include <Engine.grpc.pb.h>

namespace engine {

class EngineInterface;

class RpcServer final : public Davidian::engine::Server::Service{
public:
  RpcServer(std::unique_ptr<EngineInterface>&& engine);

  ::grpc::Status LoadFile(::grpc::ServerContext* context,
                          const ::Davidian::engine::LoadRequest* request,
                          ::Davidian::engine::LoadResponse* response);

private:
  std::unique_ptr<EngineInterface> m_engine;
};

} // namespace engine


#endif //DAVIDIAN_RPCSERVER_H
