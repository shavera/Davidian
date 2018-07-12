import os

from PySide2 import QtCore
from PySide2.QtCore import QFile, QObject
from PySide2.QtGui import QDoubleValidator
from PySide2.QtUiTools import QUiLoader
from PySide2.QtWidgets import QComboBox, QGroupBox, QLineEdit

from client import Orbital_pb2 as Davidian_orbital


class AddBodyDialog(QObject):
    add_root_body = QtCore.Signal(str, float)
    add_body = QtCore.Signal(Davidian_orbital.Body)

    def __init__(self):
        super(AddBodyDialog, self).__init__()
        dirpath = os.path.dirname(os.path.abspath(__file__))
        file = QFile(dirpath + "/add_body_dialog.ui")
        file.open(QFile.ReadOnly)

        loader = QUiLoader()
        self.dialog = loader.load(file)

        self.mass_edit = self.dialog.findChild(QLineEdit, "massEdit")
        mass_validator = QDoubleValidator(self.mass_edit)
        self.mass_edit.setValidator(mass_validator)

        self.dialog.accepted.connect(self.on_accepted)

    def set_parent_list(self, parent_list: list):
        """Set the parent selection combobox with a list of potential parent bodies. If this list is empty, then this \
        must be a root object"""
        orbit_group = self.dialog.findChild(QGroupBox, "orbitInfoBox")
        orbit_group.setEnabled(0 != len(parent_list))
        if 0 != len(parent_list):
            parent_box = self.dialog.findChild(QComboBox, "parentBodySelectorBox")
            parent_box.addItems(parent_list)

    def show(self):
        """Display the dialog"""
        self.dialog.show()

    def on_accepted(self):
        """Emit a signal containing the information in an accepted dialog"""
        name_edit = self.dialog.findChild(QLineEdit, "nameEdit")
        body = Davidian_orbital.Body()
        body.name = name_edit.text()
        body.mass = float(self.mass_edit.text())
        body.root_body.SetInParent()

        self.add_root_body.emit(name_edit.text(), float(self.mass_edit.text()))
        self.add_body.emit(body)
