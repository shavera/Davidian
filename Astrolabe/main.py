# from client import davidian_client
import sys

from PySide2.QtWidgets import QApplication

from system_ui import main_ui

def main():
    app = QApplication(sys.argv)
    editor = main_ui.SystemEditor()
    app.exec_()



if __name__ == "__main__":
    main()
