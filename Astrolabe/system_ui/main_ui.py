from PySide2.QtUiTools import QUiLoader
from PySide2.QtWidgets import QPushButton
from PySide2.QtCore import QFile

from system_ui import add_body


class MainWindow:
    def __init__(self):
        file = QFile("./system_ui/main_window.ui")
        file.open(QFile.ReadOnly)

        loader = QUiLoader()
        self.window = loader.load(file)

        add_body_button = self.window.findChild(QPushButton, "addBodyButton")
        add_body_button.clicked.connect(self.display_add_body_dialog)

    def show(self):
        self.window.show()

    def display_add_body_dialog(self):
        self._add_body_dialog = add_body.AddBodyDialog()
        self._add_body_dialog.add_root_body.connect(self.on_add_body)
        self._add_body_dialog.show()

    def on_add_body(self, name: str, mass: float):
        print(name)
        print(mass)
