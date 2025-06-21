#!/usr/bin/env python3

import gi

#gi.require_version('Notify', '0.7')
from gi.repository import Gtk, Notify

class MyApp:
    def __init__(self):
        self.notify = Notify.Notification.new(
            "My App", "My application is running", "my-app-icon.png")
        self.notify.show()

        # ... your application logic ...

    def run(self):
        Gtk.main()

if __name__ == "__main__":
    app = MyApp()
    app.run()