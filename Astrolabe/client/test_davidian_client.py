import unittest
from unittest.mock import patch, sentinel

from client.davidian_client import DavidianClient
from client import Engine_pb2 as Davidian_Engine, Orbital_pb2 as Davidian_Orbital
from client import Engine_pb2_grpc

test_system = Davidian_Engine.System()
kerbol = test_system.body.add(name="Kerbol", mass=1.7565459e28)
kerbol.root_body.SetInParent()

kerbin = test_system.body.add(name="Kerbin", mass=5.2915158e22)
kerbin_body_data = Davidian_Orbital.BodyData(parent_body_name="Kerbol")
kerbin_orbit = Davidian_Orbital.OrbitalElements(semimajor_axis=13599840256,
                                                eccentricity=0.0,
                                                longitude_asc_node=0.0,
                                                inclination=0.0,
                                                arg_periapsis=0.0,
                                                mean_anomaly_0=0.0)
kerbin_body_data.orbit.CopyFrom(kerbin_orbit)
kerbin.celestial_body.CopyFrom(kerbin_body_data)

ship = test_system.body.add(name="Ship", mass=1e5)
ship_body_data = Davidian_Orbital.BodyData(parent_body_name="Kerbin")
ship_orbit = Davidian_Orbital.OrbitalElements(semimajor_axis=750000,
                                              eccentricity=0.1,
                                              longitude_asc_node=10,
                                              inclination=20,
                                              arg_periapsis=30,
                                              mean_anomaly_0=40)
ship_body_data.orbit.CopyFrom(ship_orbit)
ship.free_body.CopyFrom(ship_body_data)


@patch('client.Engine_pb2_grpc.ServerStub')
@patch('grpc.insecure_channel')
class DavidianClientConstructorTest(unittest.TestCase):
    def test_client_constructor(self, MockChannel, MockStub):
        channel_sentinel = sentinel.some_object
        MockChannel.return_value = channel_sentinel
        stub_sentinel = sentinel.some_object
        MockStub.return_value = stub_sentinel

        client = DavidianClient()

        MockChannel.assert_called_once_with('localhost:12345')
        self.assertIs(client.channel, channel_sentinel)
        MockStub.assert_called_once_with(channel_sentinel)
        self.assertIs(client.stub, stub_sentinel)


@patch('client.Engine_pb2_grpc.ServerStub')
class DavidianClientTest(unittest.TestCase):
    def setUp(self):
        self.client = DavidianClient()

    def test_save_current_system(self, MockStub):
        self.client.stub = MockStub
        return_setinel = sentinel.some_object
        _filename = "SaveFileName"
        MockStub.SaveCurrentSystemToFile.return_value = return_setinel
        expected_request = Davidian_Engine.SaveCurrentSystemRequest(filename=_filename)

        actual_response = self.client.save_current_system(_filename)

        MockStub.SaveCurrentSystemToFile.assert_called_once_with(expected_request)
        self.assertEqual(return_setinel, actual_response)

    def test_load_system(self, MockStub):
        self.client.stub = MockStub
        MockStub.LoadSystem.return_value = test_system
        _filename = "TestFile"
        expected_request = Davidian_Engine.LoadRequest(filename=_filename)

        actual_system = self.client.load_system(_filename)

        MockStub.LoadSystem.assert_called_once_with(expected_request)
        self.assertEqual(test_system, actual_system)

    def test_get_current_system(self, MockStub):
        self.client.stub = MockStub
        MockStub.GetCurrentSystem.return_value = test_system
        expected_request = Davidian_Engine.GetCurrentSystemRequest()

        actual_system = self.client.get_current_system()

        MockStub.GetCurrentSystem.assert_called_once_with(expected_request)
        self.assertEqual(test_system, actual_system)

