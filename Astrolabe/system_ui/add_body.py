import os
import unittest

from PySide2 import QtCore
from PySide2.QtCore import QFile, QObject
from PySide2.QtUiTools import QUiLoader
from PySide2.QtWidgets import QApplication, QComboBox, QGroupBox, QLineEdit


class AddBodyDialog(QObject):
    add_root_body = QtCore.Signal(str, float)

    def __init__(self):
        super(AddBodyDialog, self).__init__()
        dirpath = os.path.dirname(os.path.abspath(__file__))
        file = QFile(dirpath + "/add_body_dialog.ui")
        file.open(QFile.ReadOnly)

        loader = QUiLoader()
        self.dialog = loader.load(file)

        self.dialog.accepted.connect(self.on_accepted)

    def set_parent_list(self, parent_list: list):
        """Set the parent selection combobox with a list of potential parent bodies. If this list is empty, then this \
        must be a root object"""
        if 0 != parent_list.count:
            orbit_group = self.dialog.findChild(QGroupBox, "orbitInfoBox")
            orbit_group.setEnabled(True)
            parent_box = self.dialog.findChild(QComboBox, "parentBodySelectorBox")
            parent_box.addItems(parent_list)

    def show(self):
        """Display the dialog"""
        self.dialog.show()

    def on_accepted(self):
        """Emit a signal containing the information in an accepted dialog"""
        name_edit = self.dialog.findChild(QLineEdit, "nameEdit")

        self.add_root_body.emit(name_edit.text(), 123.54)


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
        self.add_body_dialog = AddBodyDialog()

    def test_initial_state(self):
        orbit_group = self.add_body_dialog.dialog.findChild(QGroupBox, "orbitInfoBox")
        self.assertIsNotNone(orbit_group)
        self.assertFalse(orbit_group.isEnabled())
        pass

    def test_set_parent_list(self):
        pass


if __name__ == '__main__':
    unittest.main()
