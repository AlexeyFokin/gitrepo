#!/usr/bin/env python3


import gi
gi.require_version("Gtk", "3.0")
gi.require_version('AppIndicator3', '0.1')                              # sudo apt search gir1.2-appindicator3-0.1
from gi.repository import Gtk, AppIndicator3 as appindicator


class MyWindow(Gtk.Window):
    """My first Gtk wiundow"""
    def __init__(self):
        super().__init__(title="hehehe")


        #self.set_icon_from_file("/usr/share/icons/Adwaita/256x256/legacy/face-smile.png")


        # hb = Gtk.HeaderBar()
        # hb.set_decoration_layout("menu:minimize,maximize,close")
        # hb.set_show_close_button(True) 
        # hb.props.title = "Geeks for Geeks"
        # self.set_titlebar(hb)
        # self.set_decorated(True)  # Add this line

        # Create a notebook
        self.notebook = Gtk.Notebook()
        self.add(self.notebook)

        # Create the page 1
        self.page1 = Gtk.Box()
        self.page1.set_border_width(50)
        self.page1.add(Gtk.Label(label="This is the content of page 1"))
        self.notebook.append_page(self.page1, Gtk.Label(label="Page 1"))

        self.box = Gtk.VBox(border_width=10)

        for i in range(10):
            button = Gtk.Button(label=f'{i}')
            button.set_size_request(100, -1)

            #self.button1.connect("clicked", self.on_button1_clicked)
            self.page1.add(button)
            # self.box.pack_start(button, False, False, 0)


        # Create the page 2
        dbox = Gtk.Box()
        self.page2 = Gtk.VBox()
        dbox.pack_start(self.page2, False, False, 0)

        self.page2.set_border_width(50)
        self.page2.add(Gtk.Label(label="This is the content of page 2"))
        self.notebook.append_page(dbox, Gtk.Label(label="Page 2"))



        radiobutton1 = Gtk.RadioButton(label="RadioButton 1")
        #radiobutton1.connect("toggled", self.on_radio_button_toggled)
        self.page2.pack_start(radiobutton1, False, False, 0)
        radiobutton2 = Gtk.RadioButton(label="RadioButton 2", group=radiobutton1)
        #radiobutton2.connect("toggled", self.on_radio_button_toggled)
        self.page2.pack_start(radiobutton2, False, False, 0)

        b = Gtk.VBox()
        dbox.pack_start(b, False, False, 0)
        switch = Gtk.Switch()
        switch.set_active(False)
        switch.connect("notify::active", self.on_switch_activated)
        b.pack_start(switch, False, False, 0)

    def on_switch_activated(self, switch, gparam):
        if switch.get_active():
            state = "on"
        else:
            state = "off"
        print("Switch was turned", state)   

    def on_button1_clicked(self, widget):
        print("Hello")

    def on_button2_clicked(self, widget):
        print("Goodbye")



indicator = appindicator.Indicator.new('myappindicator',
                                       "/usr/share/icons/Adwaita/256x256/legacy/face-smile.png",
                                       appindicator.IndicatorCategory.SYSTEM_SERVICES)
indicator.set_status(appindicator.IndicatorStatus.ACTIVE)
menu = Gtk.Menu()
item_quit = Gtk.MenuItem(label="New")
menu.append(item_quit)
menu.append(Gtk.MenuItem(label="Newswws"))
indicator.set_menu(menu)
menu.show_all()

win = MyWindow()
win.connect("destroy", Gtk.main_quit)
win.show_all()
Gtk.main()
