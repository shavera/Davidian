import unittest
from unittest.mock import patch

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
class DavidianClientTest(unittest.TestCase):
    def setUp(self):
        self.client = DavidianClient()

    def test_load_system(self, MockStub):
        self.client.stub = MockStub
        self.assertIs(self.client.stub, MockStub)
        MockStub.LoadSystem.return_value = test_system
        _filename = "TestFile"
        actual_system = self.client.load_file(_filename)

        expected_request = Davidian_Engine.LoadRequest(filename=_filename)

        MockStub.LoadSystem.assert_called_once_with(expected_request)
        self.assertEqual(test_system, actual_system)
