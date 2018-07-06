#include "Server/RpcServerInterface.h"

int main() {
  auto rpcServer = server::createDefaultRpcServer();
  rpcServer->startServer("localhost:12345");
  return 0;
}