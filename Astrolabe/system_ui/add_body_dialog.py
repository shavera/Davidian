from PySide2.QtUiTools import QUiLoader
from PySide2.QtWidgets import QGroupBox, QComboBox
from PySide2.QtCore import QFile


class AddBodyDialog:
    def __init__(self):
        file = QFile("./system_ui/add_body_dialog.ui")
        file.open(QFile.ReadOnly)

        loader = QUiLoader()
        self.dialog = loader.load(file)

    def set_parent_list(self, parent_list: list):
        if 0 != parent_list.count:
            orbit_group = self.dialog.findChild(QGroupBox, "orbitInfoBox")
            orbit_group.setEnabled(True)
            parent_box = self.dialog.findChild(QComboBox, "parentBodySelectorBox")
            parent_box.addItems(parent_list)

    def show(self):
        self.dialog.show()
