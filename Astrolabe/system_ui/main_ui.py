from PySide2.QtUiTools import QUiLoader
from PySide2.QtCore import QFile


class MainWindow():
    def __init__(self):
        file = QFile("./system_ui/main_window.ui")
        file.open(QFile.ReadOnly)

        loader = QUiLoader()
        self.window = loader.load(file)
