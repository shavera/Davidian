# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'add_body_dialog.ui',
# licensing of 'add_body_dialog.ui' applies.
#
# Created: Tue Aug  7 17:47:07 2018
#      by: pyside2-uic  running on PySide2 5.11.0
#
# WARNING! All changes made in this file will be lost!

from PySide2 import QtCore, QtGui, QtWidgets

class Ui_AddBodyDialog(object):
    def setupUi(self, AddBodyDialog):
        AddBodyDialog.setObjectName("AddBodyDialog")
        AddBodyDialog.resize(465, 467)
        self.verticalLayout = QtWidgets.QVBoxLayout(AddBodyDialog)
        self.verticalLayout.setObjectName("verticalLayout")
        self.unitsLabel = QtWidgets.QLabel(AddBodyDialog)
        self.unitsLabel.setObjectName("unitsLabel")
        self.verticalLayout.addWidget(self.unitsLabel)
        self.bodyInfoBox = QtWidgets.QGroupBox(AddBodyDialog)
        self.bodyInfoBox.setObjectName("bodyInfoBox")
        self.formLayout = QtWidgets.QFormLayout(self.bodyInfoBox)
        self.formLayout.setObjectName("formLayout")
        self.nameEdit = QtWidgets.QLineEdit(self.bodyInfoBox)
        self.nameEdit.setObjectName("nameEdit")
        self.formLayout.setWidget(0, QtWidgets.QFormLayout.FieldRole, self.nameEdit)
        self.nameLabel = QtWidgets.QLabel(self.bodyInfoBox)
        self.nameLabel.setObjectName("nameLabel")
        self.formLayout.setWidget(0, QtWidgets.QFormLayout.LabelRole, self.nameLabel)
        self.massLabel = QtWidgets.QLabel(self.bodyInfoBox)
        self.massLabel.setObjectName("massLabel")
        self.formLayout.setWidget(1, QtWidgets.QFormLayout.LabelRole, self.massLabel)
        self.massEdit = QtWidgets.QLineEdit(self.bodyInfoBox)
        self.massEdit.setObjectName("massEdit")
        self.formLayout.setWidget(1, QtWidgets.QFormLayout.FieldRole, self.massEdit)
        self.verticalLayout.addWidget(self.bodyInfoBox)
        self.orbitInfoBox = QtWidgets.QGroupBox(AddBodyDialog)
        self.orbitInfoBox.setEnabled(False)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Preferred, QtWidgets.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(1)
        sizePolicy.setHeightForWidth(self.orbitInfoBox.sizePolicy().hasHeightForWidth())
        self.orbitInfoBox.setSizePolicy(sizePolicy)
        self.orbitInfoBox.setObjectName("orbitInfoBox")
        self.formLayout_2 = QtWidgets.QFormLayout(self.orbitInfoBox)
        self.formLayout_2.setObjectName("formLayout_2")
        self.parentNameLabel = QtWidgets.QLabel(self.orbitInfoBox)
        self.parentNameLabel.setObjectName("parentNameLabel")
        self.formLayout_2.setWidget(1, QtWidgets.QFormLayout.LabelRole, self.parentNameLabel)
        self.parentBodySelectorBox = QtWidgets.QComboBox(self.orbitInfoBox)
        self.parentBodySelectorBox.setObjectName("parentBodySelectorBox")
        self.formLayout_2.setWidget(1, QtWidgets.QFormLayout.FieldRole, self.parentBodySelectorBox)
        self.semiMajorAxisLabel = QtWidgets.QLabel(self.orbitInfoBox)
        self.semiMajorAxisLabel.setObjectName("semiMajorAxisLabel")
        self.formLayout_2.setWidget(2, QtWidgets.QFormLayout.LabelRole, self.semiMajorAxisLabel)
        self.semiMajorAxisEdit = QtWidgets.QLineEdit(self.orbitInfoBox)
        self.semiMajorAxisEdit.setObjectName("semiMajorAxisEdit")
        self.formLayout_2.setWidget(2, QtWidgets.QFormLayout.FieldRole, self.semiMajorAxisEdit)
        self.eccentricityLabel = QtWidgets.QLabel(self.orbitInfoBox)
        self.eccentricityLabel.setObjectName("eccentricityLabel")
        self.formLayout_2.setWidget(3, QtWidgets.QFormLayout.LabelRole, self.eccentricityLabel)
        self.eccentricitySpinBox = QtWidgets.QDoubleSpinBox(self.orbitInfoBox)
        self.eccentricitySpinBox.setMaximum(10000.0)
        self.eccentricitySpinBox.setObjectName("eccentricitySpinBox")
        self.formLayout_2.setWidget(3, QtWidgets.QFormLayout.FieldRole, self.eccentricitySpinBox)
        self.inclinationLabel = QtWidgets.QLabel(self.orbitInfoBox)
        self.inclinationLabel.setObjectName("inclinationLabel")
        self.formLayout_2.setWidget(4, QtWidgets.QFormLayout.LabelRole, self.inclinationLabel)
        self.inclinationSpinBox = QtWidgets.QDoubleSpinBox(self.orbitInfoBox)
        self.inclinationSpinBox.setMinimum(-180.0)
        self.inclinationSpinBox.setMaximum(180.0)
        self.inclinationSpinBox.setObjectName("inclinationSpinBox")
        self.formLayout_2.setWidget(4, QtWidgets.QFormLayout.FieldRole, self.inclinationSpinBox)
        self.longitudeAscNodeLabel = QtWidgets.QLabel(self.orbitInfoBox)
        self.longitudeAscNodeLabel.setObjectName("longitudeAscNodeLabel")
        self.formLayout_2.setWidget(5, QtWidgets.QFormLayout.LabelRole, self.longitudeAscNodeLabel)
        self.longitudeAscNodeSpinBox = QtWidgets.QDoubleSpinBox(self.orbitInfoBox)
        self.longitudeAscNodeSpinBox.setMinimum(-180.0)
        self.longitudeAscNodeSpinBox.setMaximum(180.0)
        self.longitudeAscNodeSpinBox.setObjectName("longitudeAscNodeSpinBox")
        self.formLayout_2.setWidget(5, QtWidgets.QFormLayout.FieldRole, self.longitudeAscNodeSpinBox)
        self.argOfPeriapsisLabel = QtWidgets.QLabel(self.orbitInfoBox)
        self.argOfPeriapsisLabel.setObjectName("argOfPeriapsisLabel")
        self.formLayout_2.setWidget(6, QtWidgets.QFormLayout.LabelRole, self.argOfPeriapsisLabel)
        self.argOfPeriapsisSpinBox = QtWidgets.QDoubleSpinBox(self.orbitInfoBox)
        self.argOfPeriapsisSpinBox.setMinimum(-180.0)
        self.argOfPeriapsisSpinBox.setMaximum(180.0)
        self.argOfPeriapsisSpinBox.setObjectName("argOfPeriapsisSpinBox")
        self.formLayout_2.setWidget(6, QtWidgets.QFormLayout.FieldRole, self.argOfPeriapsisSpinBox)
        self.meanAnomalyLabel = QtWidgets.QLabel(self.orbitInfoBox)
        self.meanAnomalyLabel.setObjectName("meanAnomalyLabel")
        self.formLayout_2.setWidget(7, QtWidgets.QFormLayout.LabelRole, self.meanAnomalyLabel)
        self.meanAnomalySpinBox = QtWidgets.QDoubleSpinBox(self.orbitInfoBox)
        self.meanAnomalySpinBox.setMinimum(-180.0)
        self.meanAnomalySpinBox.setMaximum(180.0)
        self.meanAnomalySpinBox.setObjectName("meanAnomalySpinBox")
        self.formLayout_2.setWidget(7, QtWidgets.QFormLayout.FieldRole, self.meanAnomalySpinBox)
        self.celestialBodyBox = QtWidgets.QCheckBox(self.orbitInfoBox)
        self.celestialBodyBox.setChecked(True)
        self.celestialBodyBox.setObjectName("celestialBodyBox")
        self.formLayout_2.setWidget(0, QtWidgets.QFormLayout.FieldRole, self.celestialBodyBox)
        self.verticalLayout.addWidget(self.orbitInfoBox)
        self.buttonBox = QtWidgets.QDialogButtonBox(AddBodyDialog)
        self.buttonBox.setOrientation(QtCore.Qt.Horizontal)
        self.buttonBox.setStandardButtons(QtWidgets.QDialogButtonBox.Cancel|QtWidgets.QDialogButtonBox.Ok)
        self.buttonBox.setObjectName("buttonBox")
        self.verticalLayout.addWidget(self.buttonBox)

        self.retranslateUi(AddBodyDialog)
        QtCore.QObject.connect(self.buttonBox, QtCore.SIGNAL("accepted()"), AddBodyDialog.accept)
        QtCore.QObject.connect(self.buttonBox, QtCore.SIGNAL("rejected()"), AddBodyDialog.reject)
        QtCore.QMetaObject.connectSlotsByName(AddBodyDialog)

    def retranslateUi(self, AddBodyDialog):
        AddBodyDialog.setWindowTitle(QtWidgets.QApplication.translate("AddBodyDialog", "Dialog", None, -1))
        self.unitsLabel.setText(QtWidgets.QApplication.translate("AddBodyDialog", "All Units are MKS (Meters, Kilograms, Seconds); Angles are degrees (-180 .. 180) ", None, -1))
        self.bodyInfoBox.setTitle(QtWidgets.QApplication.translate("AddBodyDialog", "Body Info", None, -1))
        self.nameLabel.setText(QtWidgets.QApplication.translate("AddBodyDialog", "Name", None, -1))
        self.massLabel.setText(QtWidgets.QApplication.translate("AddBodyDialog", "Mass", None, -1))
        self.massEdit.setPlaceholderText(QtWidgets.QApplication.translate("AddBodyDialog", "kg", None, -1))
        self.orbitInfoBox.setTitle(QtWidgets.QApplication.translate("AddBodyDialog", "Orbit Info", None, -1))
        self.parentNameLabel.setText(QtWidgets.QApplication.translate("AddBodyDialog", "Parent Name", None, -1))
        self.semiMajorAxisLabel.setText(QtWidgets.QApplication.translate("AddBodyDialog", "Semi Major Axis", None, -1))
        self.eccentricityLabel.setText(QtWidgets.QApplication.translate("AddBodyDialog", "Eccentricity", None, -1))
        self.inclinationLabel.setText(QtWidgets.QApplication.translate("AddBodyDialog", "Inclination", None, -1))
        self.longitudeAscNodeLabel.setText(QtWidgets.QApplication.translate("AddBodyDialog", "Longitude of Asc. Node", None, -1))
        self.argOfPeriapsisLabel.setText(QtWidgets.QApplication.translate("AddBodyDialog", "Argument of Periapsis", None, -1))
        self.meanAnomalyLabel.setText(QtWidgets.QApplication.translate("AddBodyDialog", "Mean Anomaly at Epoch", None, -1))
        self.celestialBodyBox.setText(QtWidgets.QApplication.translate("AddBodyDialog", "Celestial Body", None, -1))
