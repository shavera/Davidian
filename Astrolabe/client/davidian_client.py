import grpc

from client import Engine_pb2_grpc, Engine_pb2


class DavidianClient:
    def __init__(self, server_address='localhost:12345'):
        self.channel = grpc.insecure_channel(server_address)
        self.stub = Engine_pb2_grpc.ServerStub(self.channel)

    def load_file(self, file: str) -> object:
        response = self.stub.LoadFile(Engine_pb2.LoadRequest(filename=file))
        return response
