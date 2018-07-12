import unittest

from PySide2.QtGui import QDoubleValidator
from PySide2.QtWidgets import QApplication, QCheckBox, QComboBox, QGroupBox, QLineEdit

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
    root_body_added = False

    def setUp(self):
        super(AddBodyDialogTest, self).setUp()
        self.add_body_dialog = add_body.AddBodyDialog()
        self.dialog = self.add_body_dialog.dialog
        self.nameEdit = self.dialog.findChild(QLineEdit, "nameEdit")
        self.massEdit = self.dialog.findChild(QLineEdit, "massEdit")
        self.orbit_group = self.dialog.findChild(QGroupBox, "orbitInfoBox")
        self.parent_box = self.dialog.findChild(QComboBox, "parentBodySelectorBox")
        self.celestial_body_box = self.dialog.findChild(QCheckBox, "celestialBodyBox")

        self.add_body_dialog.add_root_body.connect(self._on_root_body_added)

    def test_initial_state(self):
        """Test that any specific details about initial conditions are met"""
        self.assertFalse(self.orbit_group.isEnabled())
        self.assertTrue(self.celestial_body_box.isChecked())
        mass_validator = self.massEdit.validator()
        self.assertIsInstance(mass_validator, QDoubleValidator)

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

    def _on_root_body_added(self, name: str, mass: float):
        """Helper signal spy for root_body_added signal"""
        self.root_body_added = True
        self.last_emitted_name = name
        self.last_emitted_mass = mass

    def test_root_body_data_accepted(self):
        """Test that when a root body's data is entered, the add_body signal is emitted with the appropriate data"""
        name = "Kerbol"
        self.nameEdit.setText(name)
        mass = "1.7565459e28"
        self.massEdit.setText(mass)

        self.dialog.accept()

        self.assertTrue(self.root_body_added)
        self.assertEqual(name, self.last_emitted_name)
        self.assertLess(relative_error(1.7565459e28, self.last_emitted_mass), 1e-6)


if __name__ == '__main__':
    unittest.main()