import matplotlib
matplotlib.use("TkAgg")

from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2TkAgg
from matplotlib.figure import Figure

import tkinter as tk
from tkinter import ttk

#Test Functie
def function(text):
    print(text)

#Main window/app
class Window(tk.Tk):
    #Init van de class
    def __init__(self, *args, **kwargs):
        tk.Tk.__init__(self, *args, **kwargs)

        container = tk.Frame(self)
        container.pack(side="top", fill="both", expand=True)

        container.grid_rowconfigure(0, weight=1)
        container.grid_columnconfigure(0, weight=1)

        self.frames = {}

        for F in (StartPage, Page1,Page2):
            frame = F(container, self)

            self.frames[F] = frame

            frame.grid(row=0, column=0, sticky="nsew")

        self.show_frame(StartPage)

    def show_frame(self, cont):
        frame = self.frames[cont]
        frame.tkraise()


class StartPage(tk.Frame):

    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        label = tk.Label(self, text="Pagina 1")
        label.pack(padx=10)

        button1 = ttk.Button(self, text="Pagina 2", command=lambda: controller.show_frame(Page1))
        button1.pack()

        button2 = ttk.Button(self, text="Page 3", command=lambda: controller.show_frame(Page2))
        button2.pack()

class Page1(tk.Frame):

    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        label = tk.Label(self, text="Pagina 2")
        label.pack(padx=10)

        button1 = ttk.Button(self, text="Terug naar 1", command=lambda: controller.show_frame(StartPage))
        button1.pack()

        button2 = ttk.Button(self, text="Page 3", command=lambda: controller.show_frame(Page2))
        button2.pack()

class Page2(tk.Frame):

    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        label = tk.Label(self, text="Pagina 3")
        label.pack(padx=10)

        button1 = ttk.Button(self, text="Terug naar 1", command=lambda: controller.show_frame(StartPage))
        button1.pack()

        button2 = ttk.Button(self, text="Page 2", command=lambda: controller.show_frame(Page1))
        button2.pack()


app = Window()

app.mainloop()