import grpc

from client import Engine_pb2_grpc, Engine_pb2


class DavidianClient:
    def __init__(self, server_address='localhost:12345'):
        self.channel = grpc.insecure_channel(server_address)
        self.stub = Engine_pb2_grpc.ServerStub(self.channel)

    def load_system(self, file: str) -> Engine_pb2.System:
        return self.stub.LoadSystem(Engine_pb2.LoadRequest(filename=file))

    def get_current_system(self) -> Engine_pb2.System:
        return self.stub.GetCurrentSystem(Engine_pb2.GetCurrentSystemRequest())

    