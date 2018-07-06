import sys
from PySide2.QtWidgets import QApplication, QLabel

class SystemEditor:
    def __init__(self):
        self.app = QApplication(sys.argv)
        self.label = QLabel("Hello World!")
        self.label.show()
        self.app.exec_()
