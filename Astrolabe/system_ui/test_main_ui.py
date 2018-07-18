from PySide2.QtCore import QTimer
from PySide2.QtWidgets import QListWidget, QLineEdit, QPushButton

from system_ui import main_ui, test_helpers


class MainWindowTest(test_helpers.UsesQApplication):
    body_added_name = "Kerbol"
    body_added_mass = 1.7565459e28

    def setUp(self):
        super(MainWindowTest, self).setUp()
        self.main_window = main_ui.MainWindow()
        self.window = self.main_window.window

        self.list_widget = self.window.findChild(QListWidget, "bodyList")

        self.add_body_button = self.window.findChild(QPushButton, "addBodyButton")
        self.assertIsNotNone(self.add_body_button)

    def _fill_and_accept_modal_dialog(self):
        dialog = self.app.activeModalWidget()
        self.assertIsNotNone(dialog)
        dialog.findChild(QLineEdit, "nameEdit").setText(self.body_added_name)
        dialog.findChild(QLineEdit, "massEdit").setText(str(self.body_added_mass))
        dialog.accept()

    def test_add_body_button(self):
        """Test that when add body button is clicked, a dialog will appear that can be filled in with data"""
        # since dialog is modal, connect a singleshot timer to a function that will fill in the dialog and accept it
        QTimer.singleShot(0, self._fill_and_accept_modal_dialog)
        self.add_body_button.click()

        self.assertEqual(1, len(self.main_window._bodies))

        self.assertEqual(1, self.list_widget.count())
        self.assertEqual(self.body_added_name, self.list_widget.item(0).text())
