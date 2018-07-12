import unittest

from PySide2.QtWidgets import QApplication, QCheckBox, QComboBox, QGroupBox

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


class AddBodyDialogTest(UsesQApplication):
    def setUp(self):
        super(AddBodyDialogTest, self).setUp()
        self.add_body_dialog = add_body.AddBodyDialog()
        self.dialog = self.add_body_dialog.dialog
        self.orbit_group = self.dialog.findChild(QGroupBox, "orbitInfoBox")
        self.parent_box = self.dialog.findChild(QComboBox, "parentBodySelectorBox")
        self.celestial_body_box = self.dialog.findChild(QCheckBox, "celestialBodyBox")

    def test_initial_state(self):
        self.assertFalse(self.orbit_group.isEnabled())
        self.assertTrue(self.celestial_body_box.isChecked())

    def test_set_parent_list(self):
        with self.subTest("Empty List"):
            self.add_body_dialog.set_parent_list([])
            self.assertFalse(self.orbit_group.isEnabled())
        with self.subTest("Non-Empty List"):
            parent_list = ["Kerbol", "Kerbin", "Mun"]
            self.add_body_dialog.set_parent_list(parent_list)
            self.assertTrue(self.orbit_group.isEnabled())
            self.assertEqual(len(parent_list), self.parent_box.count())
            for parent in parent_list:
                self.assertNotEqual(-1, self.parent_box.findText(parent))


if __name__ == '__main__':
    unittest.main()