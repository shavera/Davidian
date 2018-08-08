from PySide2.QtWidgets import QMainWindow
from PySide2.QtCore import Slot

from client import Orbital_pb2 as Davidian_orbital
from system_ui.ui_main_window import Ui_MainWindow
from system_ui.add_body import AddBodyDialog


class MainWindow(QMainWindow):
    _bodies = []

    def __init__(self, parent):
        super(MainWindow, self).__init__(parent)

        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)

        self.ui.addBodyButton.clicked.connect(self.display_add_body_dialog)

        self.add_body_dialog = AddBodyDialog(self)
        self.add_body_dialog.add_body.connect(self._on_add_body)

    def _get_body_names(self):
        """Get a list of the current body names"""
        _body_names = []
        for body in self._bodies:
            _body_names.append(body.name)
        return _body_names

    @Slot()
    def display_add_body_dialog(self):
        """Create and show a new Add Body dialog"""
        self.add_body_dialog.set_parent_list(self._get_body_names())
        self.add_body_dialog.show()

    @Slot(Davidian_orbital.Body)
    def _on_add_body(self, body: Davidian_orbital.Body):
        """When adding the dialog signals to add a body, gather the data and create a body"""
        self._bodies.append(body)
        self.ui.bodyList.addItem(body.name)
