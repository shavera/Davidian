import unittest

from PySide2.QtWidgets import QApplication, QGroupBox

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

    def test_initial_state(self):
        orbit_group = self.add_body_dialog.dialog.findChild(QGroupBox, "orbitInfoBox")
        self.assertIsNotNone(orbit_group)
        self.assertFalse(orbit_group.isEnabled())
        pass

    def test_set_parent_list(self):
        pass


if __name__ == '__main__':
    unittest.main()