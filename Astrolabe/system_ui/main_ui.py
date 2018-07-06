from PySide2.QtUiTools import QUiLoader
from PySide2.QtWidgets import QPushButton
from PySide2.QtCore import QFile


class MainWindow():
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
        loader = QUiLoader()
        file = QFile("./system_ui/add_body_dialog.ui")
        dialog = loader.load(file, self.window)
        dialog.show()
