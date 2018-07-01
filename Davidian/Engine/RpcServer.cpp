//
// Created by alexs on 7/1/18.
//

#include "RpcServer.h"

namespace engine{

::grpc::Status RpcServer::LoadFile(::grpc::ServerContext* context, const ::Davidian::engine::LoadRequest* request,
                                   ::Davidian::engine::LoadResponse* response) {
  return Service::LoadFile(context, request, response);
}
} // namespace engine