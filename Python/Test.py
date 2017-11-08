from tkinter import *

window = Tk()

#label = Label(root, text="Tekst")
#label.pack()

topFrame = Frame(window)
topFrame.pack(side=TOP)

bottomFrame = Frame(window)
bottomFrame.pack(side=BOTTOM)

button1 = Button(topFrame, text= "Button", fg="green")
button2 = Button(topFrame, text= "Button2", fg="red")
button3 = Button(topFrame, text= "Button3", fg="blue")
button4 = Button(bottomFrame, text= "Button4", fg="red")

button1.pack
window.mainloop()
