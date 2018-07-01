//
// Created by alexs on 7/1/18.
//

#ifndef DAVIDIAN_RPCSERVERINTERFACE_H
#define DAVIDIAN_RPCSERVERINTERFACE_H

#include <memory>

namespace server{

class RpcServer;

std::unique_ptr<RpcServer> createDefaultRpcServer();

} // namespace server

#endif //DAVIDIAN_RPCSERVERINTERFACE_H
