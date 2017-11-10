import matplotlib
matplotlib.use("TkAgg")
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2TkAgg
from matplotlib.figure import Figure
from matplotlib import style

import tkinter as tk
from tkinter import ttk

style.use("ggplot")


def get_entry(self):
    print(entry1.get())

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
        button1.pack(side = "top")

        button2 = ttk.Button(self, text="Page 3", command=lambda: controller.show_frame(Page2))
        button2.pack(side = "top")

class Page1(tk.Frame):

    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        label = tk.Label(self, text="Pagina 2")
        label.pack(padx=10)

        button1 = ttk.Button(self, text="Terug naar 1", command=lambda: controller.show_frame(StartPage))
        button1.pack(side = "top",pady=10)

        button2 = ttk.Button(self, text="Page 3", command=lambda: controller.show_frame(Page2))
        button2.pack(side = "top", pady=10)

        label2 = tk.Label(self, text="Nieuwe waarde")
        label2.pack()

        entry1 = tk.Entry(self)
        entry1.pack()

        button3 = tk.Button(self, text="Invoeren", command=lambda: get_entry(self))
        button3.pack()

        def get_entry(self):  #Werkt nog niet
            v = entry1.get()
            if v in '0123456789.':
                print("Input mag geen string zijn")

            else:
                print("Waarde moet een int of float zijn")

class Page2(tk.Frame):

    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        label = tk.Label(self, text="Grafiek!")
        label.pack(pady=10, padx=10)

        button1 = ttk.Button(self, text="Back to Home",
                             command=lambda: controller.show_frame(StartPage))
        button1.pack()

        f = Figure(figsize=(5, 5), dpi=100)
        a = f.add_subplot(111)
        a.plot([1, 2, 3, 4, 5, 6, 7, 8], [5, 6, 1, 3, 3213, 9, 3, 5]) #2e tuple is de input
                                                                      #Eerste tuple is de x-as
        canvas = FigureCanvasTkAgg(f, self)
        canvas.show()
        canvas.get_tk_widget().pack(side=tk.BOTTOM, fill=tk.BOTH, expand=True)

        toolbar = NavigationToolbar2TkAgg(canvas, self)
        toolbar.update()
        canvas._tkcanvas.pack(side=tk.TOP, fill=tk.BOTH, expand=True)

app = Window()

app.mainloop()