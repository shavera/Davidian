from PySide2.QtCore import Signal
from PySide2.QtGui import QDoubleValidator
from PySide2.QtWidgets import QDialog

from client import Orbital_pb2 as Davidian_orbital

from system_ui.ui_add_body_dialog import Ui_AddBodyDialog


class AddBodyDialog(QDialog):
    add_body = Signal(Davidian_orbital.Body)

    def __init__(self, parent):
        super(AddBodyDialog, self).__init__(parent)

        self.ui = Ui_AddBodyDialog()
        self.ui.setupUi(self)

        mass_validator = QDoubleValidator(self.ui.massEdit)
        mass_validator.setBottom(0)
        self.ui.massEdit.setValidator(mass_validator)

        semimajor_axis_validator = QDoubleValidator(self.ui.semiMajorAxisEdit)
        semimajor_axis_validator.setBottom(0)
        self.ui.semiMajorAxisEdit.setValidator(semimajor_axis_validator)

    def set_parent_list(self, parent_list: list):
        """Set the parent selection combobox with a list of potential parent bodies. If this list is empty, then this \
        must be a root object"""
        self.ui.orbitInfoBox.setEnabled(0 != len(parent_list))
        if 0 != len(parent_list):
            self.ui.parentBodySelectorBox.addItems(parent_list)

    def accept(self, *args, **kwargs):
        body = Davidian_orbital.Body()
        body.name = self.ui.nameEdit.text()
        body.mass = float(self.ui.massEdit.text())

        if self.ui.orbitInfoBox.isEnabled():
            is_celestial = self.ui.celestialBodyBox.isChecked()
            body_data = body.celestial_body if is_celestial else body.free_body
            body_data.parent_body_name = self.ui.parentBodySelectorBox.currentText()
            orbit = body_data.orbit
            orbit.semimajor_axis = float(self.ui.semiMajorAxisEdit.text())
            orbit.eccentricity = self.ui.eccentricitySpinBox.value()
            orbit.inclination = self.ui.inclinationSpinBox.value()
            orbit.longitude_asc_node = self.ui.longitudeAscNodeSpinBox.value()
            orbit.arg_periapsis = self.ui.argOfPeriapsisSpinBox.value()
            orbit.mean_anomaly_0 = self.ui.meanAnomalySpinBox.value()
        else:
            body.root_body.SetInParent()

        self.add_body.emit(body)
        super(AddBodyDialog, self).accept()
