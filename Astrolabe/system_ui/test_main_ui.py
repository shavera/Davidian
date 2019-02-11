from unittest.mock import patch

from PySide2.QtWidgets import QWidget

from client import Engine_pb2 as Davidian_Engine, Orbital_pb2 as Davidian_Orbital
from system_ui import main_ui, test_helpers
from system_ui.add_body import AddBodyDialog


def create_system():
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

    return test_system


class MainWindowTest(test_helpers.UsesQApplication):
    def setUp(self):
        super(MainWindowTest, self).setUp()

        self.show_patcher = patch.object(QWidget, 'show')
        self.mock_show = self.show_patcher.start()
        self.addCleanup(self.show_patcher.stop)

        self.set_parent_list_patcher = patch.object(AddBodyDialog, 'set_parent_list')
        self.mock_set_parent_list = self.set_parent_list_patcher.start()
        self.addCleanup(self.set_parent_list_patcher.stop)

        self.window = main_ui.MainWindow(None)

    def _get_dialog(self):
        return self.window.findChild(AddBodyDialog)

    def _button_click_test(self, body_names):
        self.window.ui.addBodyButton.click()

        self.mock_set_parent_list.assert_called_once_with(body_names)
        self.mock_show.assert_called_once()

        self._get_dialog().reject()

    def test_add_body_button_clicked_empty_list(self):
        """Test what happens when the add body button is clicked with no bodies"""
        _expected_body_names = []
        self._button_click_test(_expected_body_names)

    def test_add_body_button_clicked_existing_bodies(self):
        """Test what happens when the add body button is clicked with some bodies already added"""
        _expected_body_names = ["Kerbol", "Kerbin", "Mun"]
        for name in _expected_body_names:
            body = Davidian_orbital.Body()
            body.name = name
            self.window._bodies.append(body)
            self.window.ui.bodyList.addItem(name)
        self._button_click_test(_expected_body_names)

    def test_add_body_signal(self):
        self.window.ui.addBodyButton.click()
        dialog = self._get_dialog()

        body = Davidian_orbital.Body()
        body.name = "Kerbol"
        body.mass = 1.7565459e28
        body.root_body.SetInParent()

        dialog.add_body.emit(body)

        self.assertEqual([body], self.window._bodies)
        self.assertEqual(1, self.window.ui.bodyList.count())
        self.assertEqual(body.name, self.window.ui.bodyList.item(0).text())

    def test_save_system_signal(self):
        test_system = create_system()
        self.window._bodies = [test_system.body[:]]

