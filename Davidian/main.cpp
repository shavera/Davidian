#include <iostream>
#include "Server/RpcServerInterface.h"

int main() {
  auto rpcServer = server::createDefaultRpcServer();
  std::cout << "Hello, World!" << std::endl;
  return 0;
}