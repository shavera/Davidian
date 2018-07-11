# from client import davidian_client
import sys

from PySide2.QtWidgets import QApplication

from system_ui import main_ui


def main():
    app = QApplication(sys.argv)
    main_window = main_ui.MainWindow()
    main_window.show()
    exit_code: 	int = app.exec_()
    sys.exit(exit_code)


if __name__ == "__main__":
    main()
