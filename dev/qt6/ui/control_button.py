import asyncio
import traceback

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
    QSizePolicy
)



class ControlButtons(QHBoxLayout):
    def __init__(self):
        super().__init__()

        self.lock = asyncio.Lock()

        self.addStretch(1)
        btn = QPushButton("red")
        #btn.pressed.connect(self.activate_tab_1)
        btn.setSizePolicy(QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Minimum)
        self.addWidget(btn)
        btn.clicked.connect(lambda: asyncio.ensure_future(self.button_clicked()))


        self.addStretch(1)

        btn = QPushButton("green")
        #btn.pressed.connect(self.activate_tab_2)
        btn.setSizePolicy(QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Minimum)
        self.addWidget(btn)


        self.addStretch(1)

        btn = QPushButton("yellow")
        #btn.pressed.connect(self.activate_tab_3)
        btn.setSizePolicy(QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Minimum)
        self.addWidget(btn)

        self.addStretch(1)

    async def button_clicked(self):
        try:

            if not self.lock.locked():
                async with self.lock:
                    print("starting long function")
                    await asyncio.sleep(3)
                    print("stopping long function")
        except Exception as e:
            print(e)

            print(traceback.format_exc())

