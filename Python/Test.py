import matplotlib
matplotlib.use("TkAgg")
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2TkAgg
from matplotlib.figure import Figure
from matplotlib import style
import matplotlib.animation as animation
import time
import serial
import numpy as np
from matplotlib import pyplot as plt
import tkinter as tk
from tkinter import ttk
import json

style.use("ggplot")
data = {}
def get_entry(self):
    print(entry1.get())





#Main window/app
class Window(tk.Tk):
    #Init van de class
    def __init__(self, *args, **kwargs):
        tk.Tk.__init__(self, *args, **kwargs)

        container = tk.Frame(self)
        container.grid(column='0', row='0')

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
        label = tk.Label(self, text="Setup", anchor='center')
        label.config(font=(25))
        label.grid(column='0', row='0', pady='3')

        button1 = ttk.Button(self, text="Pagina 2", command=lambda: controller.show_frame(Page1))
        button1.grid(column='0', row='1', pady='10')

        button2 = ttk.Button(self, text="Pagina 3", command=lambda: controller.show_frame(Page2))
        button2.grid(column='1', row='1', pady='10')

        label5 = tk.Label(self, text='Temperatuursensor')
        label5.config(font='15')
        label5.grid(row='2')

        labelUitrol = tk.Label(self, text='Uitrolwaarde')
        labelUitrol.grid(row='3', column='0', padx='10', pady='4')

        entryUitrol = tk.Entry(self)
        entryUitrol.grid(row='3', column='1', padx='5', pady='4')

        buttonTemp = tk.Button(self, text="Invoeren", command=lambda: writeEntrysLicht(self))
        buttonTemp.grid(row='3', column='2', padx='10', pady='4')

        labelInrol = tk.Label(self, text='Inrolwaarde')
        labelInrol.grid(row='4', column='0', padx='10', pady='4')

        entryInrol = tk.Entry(self)
        entryInrol.grid(row='4', column='1', padx='5', pady='4')

        labelAang = tk.Label(self, text='Aangesloten')
        labelAang.grid(row='5', column='0', padx='10', pady='4')

        entryAang = tk.Entry(self)
        entryAang.grid(row='5', column='1', padx='5', pady='4')

        labelPort = tk.Label(self, text='Port')
        labelPort.grid(row='6', column='0', padx='10', pady='4')

        entryPort = tk.Entry(self)
        entryPort.grid(row='6', column='1', padx='5', pady='10')

        label5 = tk.Label(self, text='Lichtsensor')
        label5.config(font='15')
        label5.grid(row='7')

        labelUitrol1 = tk.Label(self, text='Uitrolwaarde')
        labelUitrol1.grid(row='8', column='0', padx='10', pady='4')

        entryUitrol1 = tk.Entry(self)
        entryUitrol1.grid(row='8', column='1', padx='5', pady='4')

        button4 = tk.Button(self, text="Invoeren", command=lambda: writeEntrysTemp(self))
        button4.grid(row='8', column='2', padx='10', pady='4')

        labelInrol1 = tk.Label(self, text='Inrolwaarde')
        labelInrol1.grid(row='9', column='0', padx='10', pady='4')

        entryInrol1 = tk.Entry(self)
        entryInrol1.grid(row='9', column='1', padx='5', pady='4')

        labelAang1 = tk.Label(self, text='Aangesloten')
        labelAang1.grid(row='10', column='0', padx='10', pady='4')

        entryAang1 = tk.Entry(self)
        entryAang1.grid(row='10', column='1', padx='5', pady='4')

        labelPort1 = tk.Label(self, text='Port')
        labelPort1.grid(row='11', column='0', padx='10', pady='4')

        entryPort1 = tk.Entry(self)
        entryPort1.grid(row='11', column='1', padx='5', pady='10')

        def writeEntrysLicht(self):
            data['licht'] = []
            data['licht'].append({
                'Uitrol': entryUitrol.get(),
                'Inrol': entryInrol.get(),
                'Aang': entryAang.get(),
                'Port':entryPort.get()
            })
            print(data['licht'])
            with open("PythonConfig.json", "w") as fp:
                json.dump(data, fp)

        def writeEntrysTemp(self):
            data['temp'] = []
            data['temp'].append({
                'Uitrol': entryUitrol1.get(),
                'Inrol': entryInrol1.get(),
                'Aang': entryAang1.get(),
                'Port':entryPort1.get()
            })
            print(data['temp'])
            with open("PythonConfig.json", "w") as fp:
                json.dump(data, fp)

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

        entry1 = tk.Entry(self, text='Getal invoeren')
        entry1.pack()

        button3 = tk.Button(self, text="Invoeren", command=lambda: write_entry(self))
        button3.pack()


class Page2(tk.Frame):

    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        label = tk.Label(self, text="Grafiek!")
        label.pack(pady=10, padx=10)

        button1 = ttk.Button(self, text="Back to Home",
                             command=lambda: controller.show_frame(StartPage))
        button1.pack()
'''
        ser = serial.Serial()
        ser.baudrate = 19200
        ser.port = 'COM4'
        ser.open()

        plt.ion()  # set plot to animated

        ydata = [0] * 50
        ax1 = plt.axes()

        # make plot
        line, = plt.plot(ydata)
        plt.ylim([10, 40])

        # start data collection
        while True:
            data = ser.readline().rstrip()  # read data from serial
            # port and strip line endings

            ymin = float(min(ydata)) - 10
            ymax = float(max(ydata)) + 10
            plt.ylim([ymin, ymax])
            ydata.append(data)
            del ydata[0]
            line.set_xdata(np.arange(len(ydata)))
            line.set_ydata(ydata)  # update the data
            plt.draw()  # update the plot

        
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
        '''

app = Window()
app.mainloop()