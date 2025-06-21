import sys

from PySide6 import QtAsyncio

from PySide6.QtCore import Qt
from PySide6.QtWidgets import (
    QApplication,
    QHBoxLayout,
    QLabel,
    QMainWindow,
    QPushButton,
    QStackedLayout,
    QVBoxLayout,
    QWidget,
)
from PySide6.QtGui import QPalette, QColor

from ui import control_button

class Color(QWidget):
    def __init__(self, color):
        super().__init__()
        self.setAutoFillBackground(True)

        palette = self.palette()
        palette.setColor(QPalette.Window, QColor(color))
        self.setPalette(palette)


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("My App")

 
        pagelayout = QVBoxLayout()
        button_layout = control_button.ControlButtons()

        pagelayout.addLayout(button_layout)
        pagelayout.addStretch(1)

        widget = QWidget()
        widget.setLayout(pagelayout)
        self.setCentralWidget(widget)


    def activate_tab_1(self):
        self.stacklayout.setCurrentIndex(0)

    def activate_tab_2(self):
        self.stacklayout.setCurrentIndex(1)

    def activate_tab_3(self):
        self.stacklayout.setCurrentIndex(2)


app = QApplication(sys.argv)
window = MainWindow()
window.show()
#app.exec()
QtAsyncio.run()
