import unittest

from PySide2.QtWidgets import QApplication

class UsesQApplication(unittest.TestCase):
    """Helper class to provide QApplication instances"""

    def setUp(self):
        """Creates the QApplication instance"""

        # Simple way of making instance a singleton
        super(UsesQApplication, self).setUp()
        self.app = QApplication.instance() or QApplication([])

    def tearDown(self):
        """Deletes the reference owned by self"""
        del self.app
        super(UsesQApplication, self).tearDown()


def relative_error(expected, actual) -> float:
    diff = abs(expected - actual)
    if 0 != expected:
        return diff/expected
    else:
        return diff
