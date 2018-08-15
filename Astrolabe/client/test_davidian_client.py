import unittest

from client import davidian_client


class DavidianClientTest(unittest.TestCase):
    def test_dummy(self):
        client = davidian_client.DavidianClient()
        self.assertIsNotNone(client)
