import grpc

import Engine_pb2
import Engine_pb2_grpc


class DavidianClient:
    def __init__(self, server_address='localhost:12345'):
        self.channel = grpc.insecure_channel(server_address)
        self.stub = Engine_pb2_grpc.ServerStub(self.channel)

    def load_file(self, filename: str) -> object:
        response = self.stub.LoadFile(filename)
        return response
