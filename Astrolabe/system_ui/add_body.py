import os

from PySide2 import QtCore
from PySide2.QtCore import QFile, QObject
from PySide2.QtGui import QDoubleValidator
from PySide2.QtUiTools import QUiLoader
from PySide2.QtWidgets import QCheckBox, QComboBox, QDoubleSpinBox, QGroupBox, QLineEdit

from client import Orbital_pb2 as Davidian_orbital


class AddBodyDialog(QObject):
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
        mass_validator.setBottom(0)
        self.mass_edit.setValidator(mass_validator)

        semimajor_axis_validator = QDoubleValidator(self.mass_edit)
        semimajor_axis_validator.setBottom(0)
        self.semimajor_axis_editor = self.dialog.findChild(QLineEdit, "semiMajorAxisEdit")
        self.semimajor_axis_editor.setValidator(semimajor_axis_validator)

        self.orbit_group = self.dialog.findChild(QGroupBox, "orbitInfoBox")

        self.parent_box = self.dialog.findChild(QComboBox, "parentBodySelectorBox")

        self.dialog.accepted.connect(self.on_accepted)

    def set_parent_list(self, parent_list: list):
        """Set the parent selection combobox with a list of potential parent bodies. If this list is empty, then this \
        must be a root object"""
        self.orbit_group.setEnabled(0 != len(parent_list))
        if 0 != len(parent_list):
            self.parent_box.addItems(parent_list)

    def show(self):
        """Display the dialog"""
        self.dialog.show()

    def on_accepted(self):
        """Emit a signal containing the information in an accepted dialog"""
        name_edit = self.dialog.findChild(QLineEdit, "nameEdit")
        body = Davidian_orbital.Body()
        body.name = name_edit.text()
        body.mass = float(self.mass_edit.text())

        if self.orbit_group.isEnabled():
            is_celestial = self.dialog.findChild(QCheckBox, "celestialBodyBox").isChecked()
            body_data = body.celestial_body if is_celestial else body.free_body
            body_data.parent_body_name = self.parent_box.currentText()
            orbit = body_data.orbit
            orbit.semimajor_axis = float(self.dialog.findChild(QLineEdit, "semiMajorAxisEdit").text())
            orbit.eccentricity = self.dialog.findChild(QDoubleSpinBox, "eccentricitySpinBox").value()
            orbit.inclination = self.dialog.findChild(QDoubleSpinBox, "inclinationSpinBox").value()
            orbit.longitude_asc_node = self.dialog.findChild(QDoubleSpinBox, "longitudeAscNodeSpinBox").value()
            orbit.arg_periapsis = self.dialog.findChild(QDoubleSpinBox, "argOfPeriapsisSpinBox").value()
            orbit.mean_anomaly_0 = self.dialog.findChild(QDoubleSpinBox, "meanAnomalySpinBox").value()
        else:
            body.root_body.SetInParent()

        self.add_body.emit(body)
