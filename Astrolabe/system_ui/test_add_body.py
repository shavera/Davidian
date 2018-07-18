import math
import unittest

from PySide2.QtGui import QDoubleValidator
from PySide2.QtWidgets import QApplication, QCheckBox, QComboBox, QDoubleSpinBox, QGroupBox, QLineEdit

from client import Orbital_pb2 as Davidian_orbital

from system_ui import add_body


class UsesQApplication(unittest.TestCase):
    """Helper class to provide QApplication instances"""

    def setUp(self):
        """Creates the QApplication instance"""

        # Simple way of making instance a singleton
        super(UsesQApplication, self).setUp()
        self.app = QApplication.instance() or QApplication([])

    def tearDown(self):
        """Deletes the reference owned by self"""
        del self.app
        super(UsesQApplication, self).tearDown()


def relative_error(expected, actual) -> float:
    diff = abs(expected - actual)
    if 0 != expected:
        return diff/expected
    else:
        return diff


class AddBodyDialogTest(UsesQApplication):
    def setUp(self):
        super(AddBodyDialogTest, self).setUp()
        self.add_body_dialog = add_body.AddBodyDialog()
        self.dialog = self.add_body_dialog.dialog

        self.nameEdit = self.dialog.findChild(QLineEdit, "nameEdit")
        self.assertIsNotNone(self.nameEdit)

        self.massEdit = self.dialog.findChild(QLineEdit, "massEdit")
        self.assertIsNotNone(self.massEdit)

        self.orbit_group = self.dialog.findChild(QGroupBox, "orbitInfoBox")
        self.assertIsNotNone(self.orbit_group)

        self.parent_box = self.dialog.findChild(QComboBox, "parentBodySelectorBox")
        self.assertIsNotNone(self.parent_box)

        self.celestial_body_box = self.dialog.findChild(QCheckBox, "celestialBodyBox")
        self.assertIsNotNone(self.celestial_body_box)

    def test_initial_state(self):
        """Test that any specific details about initial conditions are met"""
        with self.subTest("Orbit edit is disabled by default"):
            self.assertFalse(self.orbit_group.isEnabled())

        with self.subTest("Celestial Body is default body"):
            self.assertTrue(self.celestial_body_box.isChecked())

        with self.subTest("Mass edit has double validator with 0 as bottom limit"):
            _mass_validator = self.massEdit.validator()
            self.assertIsInstance(_mass_validator, QDoubleValidator)
            self.assertEqual(0, _mass_validator.bottom())

        with self.subTest("SemiMajor Axis edit has double validator with 0 as bottom limit"):
            _semimajor_axis_edit = self.dialog.findChild(QLineEdit, "semiMajorAxisEdit")
            _semimajor_axis_validator = _semimajor_axis_edit.validator()
            self.assertIsInstance(_semimajor_axis_validator, QDoubleValidator)
            self.assertEqual(0, _semimajor_axis_validator.bottom())

    def test_set_parent_list(self):
        """Test what happens when setting parent lists
        * Empty lists should not enable orbit group
        * Non-empty lists should enable orbit group and populate the combobox"""
        with self.subTest("Empty List"):
            self.add_body_dialog.set_parent_list([])
            self.assertFalse(self.orbit_group.isEnabled())
            self.assertEqual(0, self.parent_box.count())
        with self.subTest("Non-Empty List"):
            parent_list = ["Kerbol", "Kerbin", "Mun"]
            self.add_body_dialog.set_parent_list(parent_list)
            self.assertTrue(self.orbit_group.isEnabled())
            self.assertEqual(len(parent_list), self.parent_box.count())
            for parent in parent_list:
                self.assertNotEqual(-1, self.parent_box.findText(parent))
            self.assertEqual(parent_list[0], self.parent_box.currentText())


class AddBodyDialogAcceptedTest(AddBodyDialogTest):
    add_body_emitted = False
    signalled_body = Davidian_orbital.Body()
    parent_list = ["Kerbol", "Kerbin", "Mun"]

    def setUp(self):
        super(AddBodyDialogAcceptedTest, self).setUp()
        self.add_body_dialog.add_body.connect(self._on_add_body)

        self.semimajor_axis_edit = self.dialog.findChild(QLineEdit, "semiMajorAxisEdit")
        self.assertIsNotNone(self.semimajor_axis_edit)

        self.eccentricity_spin_box = self.dialog.findChild(QDoubleSpinBox, "eccentricitySpinBox")
        self.assertIsNotNone(self.eccentricity_spin_box)

        self.inclination_spin_box = self.dialog.findChild(QDoubleSpinBox, "inclinationSpinBox")
        self.assertIsNotNone(self.inclination_spin_box)

        self.longitude_asc_node_spin_box = self.dialog.findChild(QDoubleSpinBox, "longitudeAscNodeSpinBox")
        self.assertIsNotNone(self.longitude_asc_node_spin_box)

        self.arg_periapsis_spin_box = self.dialog.findChild(QDoubleSpinBox, "argOfPeriapsisSpinBox")
        self.assertIsNotNone(self.arg_periapsis_spin_box)

        self.mean_anomaly_spin_box = self.dialog.findChild(QDoubleSpinBox, "meanAnomalySpinBox")
        self.assertIsNotNone(self.mean_anomaly_spin_box)

    def _on_add_body(self, body):
        self.add_body_emitted = True
        self.signalled_body = body

    def test_root_body(self):
        """Test that when a root body's data is entered, the add_body signal is emitted with the appropriate data"""
        expected_body = Davidian_orbital.Body()
        expected_body.name = "Kerbol"
        expected_body.mass = 1.7565459e28
        expected_body.root_body.SetInParent()
        self.nameEdit.setText(expected_body.name)
        self.massEdit.setText(str(expected_body.mass))

        self.dialog.accept()

        self.assertTrue(self.add_body_emitted)
        self.assertEqual(expected_body, self.signalled_body)

    def test_celestial_body(self):
        """Test that when parents are present can enter data for a child celestial body"""
        self.add_body_dialog.set_parent_list(self.parent_list)
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

        self.add_body_dialog.set_parent_list(self.parent_list)
        self.nameEdit.setText(expected_body.name)
        self.massEdit.setText(str(expected_body.mass))
        self.parent_box.setCurrentIndex(self.parent_box.findText(body_data.parent_body_name))
        self.semimajor_axis_edit.setText(str(orbit.semimajor_axis))
        self.eccentricity_spin_box.setValue(orbit.eccentricity)
        self.inclination_spin_box.setValue(orbit.inclination)
        self.longitude_asc_node_spin_box.setValue(orbit.longitude_asc_node)
        self.arg_periapsis_spin_box.setValue(orbit.arg_periapsis)
        self.mean_anomaly_spin_box.setValue(orbit.mean_anomaly_0)

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

        self.add_body_dialog.set_parent_list(self.parent_list)
        self.nameEdit.setText(expected_body.name)
        self.massEdit.setText(str(expected_body.mass))
        self.parent_box.setCurrentIndex(self.parent_box.findText(body_data.parent_body_name))
        # remember to remove celestial body check here
        self.celestial_body_box.setChecked(False)
        self.semimajor_axis_edit.setText(str(orbit.semimajor_axis))
        self.eccentricity_spin_box.setValue(orbit.eccentricity)
        self.inclination_spin_box.setValue(orbit.inclination)
        self.longitude_asc_node_spin_box.setValue(orbit.longitude_asc_node)
        self.arg_periapsis_spin_box.setValue(orbit.arg_periapsis)
        self.mean_anomaly_spin_box.setValue(orbit.mean_anomaly_0)

        self.dialog.accept()

        self.assertTrue(self.add_body_emitted)
        self.assertEqual(expected_body, self.signalled_body)


if __name__ == '__main__':
    unittest.main()