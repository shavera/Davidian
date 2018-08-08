import unittest

from PySide2.QtGui import QDoubleValidator

from client import Orbital_pb2 as Davidian_orbital

from system_ui import add_body, test_helpers


class AddBodyDialogTest(test_helpers.UsesQApplication):
    def setUp(self):
        super(AddBodyDialogTest, self).setUp()
        self.dialog = add_body.AddBodyDialog(None)

    def test_initial_state(self):
        """Test that any specific details about initial conditions are met"""
        with self.subTest("Orbit edit is disabled by default"):
            self.assertFalse(self.dialog.ui.orbitInfoBox.isEnabled())

        with self.subTest("Celestial Body is default body"):
            self.assertTrue(self.dialog.ui.celestialBodyBox.isChecked())

        with self.subTest("Mass edit has double validator with 0 as bottom limit"):
            _mass_validator = self.dialog.ui.massEdit.validator()
            self.assertIsInstance(_mass_validator, QDoubleValidator)
            self.assertEqual(0, _mass_validator.bottom())

        with self.subTest("SemiMajor Axis edit has double validator with 0 as bottom limit"):
            _semimajor_axis_validator = self.dialog.ui.semiMajorAxisEdit.validator()
            self.assertIsInstance(_semimajor_axis_validator, QDoubleValidator)
            self.assertEqual(0, _semimajor_axis_validator.bottom())

    def test_set_parent_list(self):
        """Test what happens when setting parent lists
        * Empty lists should not enable orbit group
        * Non-empty lists should enable orbit group and populate the combobox"""
        with self.subTest("Empty List"):
            self.dialog.set_parent_list([])
            self.assertFalse(self.dialog.ui.orbitInfoBox.isEnabled())
            self.assertEqual(0, self.dialog.ui.parentBodySelectorBox.count())

        with self.subTest("Non-Empty List"):
            parent_list = ["Kerbol", "Kerbin", "Mun"]
            self.dialog.set_parent_list(parent_list)
            self.assertTrue(self.dialog.ui.orbitInfoBox.isEnabled())
            _parent_box = self.dialog.ui.parentBodySelectorBox
            self.assertEqual(len(parent_list), _parent_box.count())
            for parent in parent_list:
                self.assertNotEqual(-1,_parent_box.findText(parent))
            self.assertEqual(parent_list[0], _parent_box.currentText())


class AddBodyDialogAcceptedTest(AddBodyDialogTest):
    add_body_emitted = False
    signalled_body = Davidian_orbital.Body()
    parent_list = ["Kerbol", "Kerbin", "Mun"]

    def setUp(self):
        super(AddBodyDialogAcceptedTest, self).setUp()
        self.dialog.add_body.connect(self._on_add_body)

    def _on_add_body(self, body):
        self.add_body_emitted = True
        self.signalled_body = body

    def test_root_body(self):
        """Test that when a root body's data is entered, the add_body signal is emitted with the appropriate data"""
        expected_body = Davidian_orbital.Body()
        expected_body.name = "Kerbol"
        expected_body.mass = 1.7565459e28
        expected_body.root_body.SetInParent()
        self.dialog.ui.nameEdit.setText(expected_body.name)
        self.dialog.ui.massEdit.setText(str(expected_body.mass))

        self.dialog.accept()

        self.assertTrue(self.add_body_emitted)
        self.assertEqual(expected_body, self.signalled_body)

    def test_celestial_body(self):
        """Test that when parents are present can enter data for a child celestial body"""
        parent_index = 1
        expected_body = Davidian_orbital.Body()
        expected_body.name = "Minmus"
        expected_body.mass = 2.6457580e19
        body_data = expected_body.celestial_body
        orbit = body_data.orbit
        body_data.parent_body_name = self.parent_list[parent_index]
        orbit.semimajor_axis = 4.7e7
        orbit.eccentricity = 0
        orbit.inclination = 6.0
        orbit.longitude_asc_node = 78
        orbit.arg_periapsis = 38
        # 0.9 radians, rounded to 2 decimals, is 51.57
        orbit.mean_anomaly_0 = 51.57

        _parent_box = self.dialog.ui.parentBodySelectorBox
        self.dialog.set_parent_list(self.parent_list)
        self.dialog.ui.nameEdit.setText(expected_body.name)
        self.dialog.ui.massEdit.setText(str(expected_body.mass))
        _parent_box.setCurrentIndex(_parent_box.findText(body_data.parent_body_name))
        self.dialog.ui.semiMajorAxisEdit.setText(str(orbit.semimajor_axis))
        self.dialog.ui.eccentricitySpinBox.setValue(orbit.eccentricity)
        self.dialog.ui.inclinationSpinBox.setValue(orbit.inclination)
        self.dialog.ui.longitudeAscNodeSpinBox.setValue(orbit.longitude_asc_node)
        self.dialog.ui.argOfPeriapsisSpinBox.setValue(orbit.arg_periapsis)
        self.dialog.ui.meanAnomalySpinBox.setValue(orbit.mean_anomaly_0)

        self.dialog.accept()

        self.assertTrue(self.add_body_emitted)
        self.assertEqual(expected_body, self.signalled_body)

    def test_free_body(self):
        """Test that when parents are present, can deselect celestial body and enter data for a free body"""
        parent_index = 1
        expected_body = Davidian_orbital.Body()
        expected_body.name = "Ship"
        expected_body.mass = 200000
        body_data = expected_body.free_body
        orbit = body_data.orbit
        body_data.parent_body_name = self.parent_list[parent_index]
        orbit.semimajor_axis = 700000
        orbit.eccentricity = 0.03
        orbit.inclination = 13.75
        orbit.longitude_asc_node = -35.23
        orbit.arg_periapsis = 103.24
        orbit.mean_anomaly_0 = 36.6

        _parent_box = self.dialog.ui.parentBodySelectorBox
        self.dialog.set_parent_list(self.parent_list)
        self.dialog.ui.nameEdit.setText(expected_body.name)
        self.dialog.ui.massEdit.setText(str(expected_body.mass))
        _parent_box.setCurrentIndex(_parent_box.findText(body_data.parent_body_name))
        # remember to remove celestial body check here
        self.dialog.ui.celestialBodyBox.setChecked(False)
        self.dialog.ui.semiMajorAxisEdit.setText(str(orbit.semimajor_axis))
        self.dialog.ui.eccentricitySpinBox.setValue(orbit.eccentricity)
        self.dialog.ui.inclinationSpinBox.setValue(orbit.inclination)
        self.dialog.ui.longitudeAscNodeSpinBox.setValue(orbit.longitude_asc_node)
        self.dialog.ui.argOfPeriapsisSpinBox.setValue(orbit.arg_periapsis)
        self.dialog.ui.meanAnomalySpinBox.setValue(orbit.mean_anomaly_0)

        self.dialog.accept()

        self.assertTrue(self.add_body_emitted)
        self.assertEqual(expected_body, self.signalled_body)


if __name__ == '__main__':
    unittest.main()