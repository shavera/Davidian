//
// Created by alexs on 7/1/18.
//

#ifndef DAVIDIAN_RPCSERVER_H
#define DAVIDIAN_RPCSERVER_H

#include <Engine.grpc.pb.h>

namespace engine {

class RpcServer final : public Davidian::engine::Server::Service{
public:
  ::grpc::Status LoadFile(::grpc::ServerContext* context,
                          const ::Davidian::engine::LoadRequest* request,
                          ::Davidian::engine::LoadResponse* response);
};

} // namespace engine


#endif //DAVIDIAN_RPCSERVER_H
