#include <iostream>
#include "Server/RpcServerInterface.h"

int main() {
  auto rpcServer = server::createDefaultRpcServer();
  rpcServer->startServer("localhost:12345");
  std::cout << "Hello, World!" << std::endl;
  return 0;
}