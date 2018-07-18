from PySide2.QtUiTools import QUiLoader
from PySide2.QtWidgets import QListWidget, QPushButton
from PySide2.QtCore import QFile

from client import Orbital_pb2 as Davidian_orbital
from system_ui import add_body


class MainWindow:
    _bodies = []

    def __init__(self):
        file = QFile("./system_ui/main_window.ui")
        file.open(QFile.ReadOnly)

        loader = QUiLoader()
        self.window = loader.load(file)

        add_body_button = self.window.findChild(QPushButton, "addBodyButton")
        add_body_button.clicked.connect(self.display_add_body_dialog)

        self.body_list = self.window.findChild(QListWidget, "bodyList")

        self.add_body_dialog = add_body.AddBodyDialog()

    def show(self):
        """Display the main window"""
        self.window.show()

    def display_add_body_dialog(self):
        """Create and show a new Add Body dialog"""
        self.add_body_dialog.add_body.connect(self._on_add_body)
        self.add_body_dialog.show()

    def _on_add_body(self, body: Davidian_orbital.Body):
        """When adding the dialog signals to add a body, gather the data and create a body"""
        self._bodies.append(body)
        self.body_list.addItem(body.name)
