//
// Created by alexs on 7/1/18.
//

#ifndef DAVIDIAN_RPCSERVERINTERFACE_H
#define DAVIDIAN_RPCSERVERINTERFACE_H

#include <memory>

namespace server{

class RpcServerInterface{
public:
  virtual void startServer(const std::string& serverAddress) = 0;
};

std::unique_ptr<RpcServerInterface> createDefaultRpcServer();

} // namespace server

#endif //DAVIDIAN_RPCSERVERINTERFACE_H
