import serial
from tkinter import *
import tkinter as tk
import time

commPort = 'COM3'
ser = serial.Serial(commPort, baudrate=9600, timeout=1)


def turnOnValve():
    ser.write(b'o')
    setTempText(0, 3)


def turnOffValve():
    ser.write(b'x')
    setTempText(0, 2)


def setTemp():
        ser.write(b'b')
        time.sleep(1)
        numTemp = entryTemp.get()
        setTempText(numTemp, 1)
        dataToSend = numTemp
        ser.write(dataToSend.encode())


def setTempText(x, y):
    settemplabel = tk.Label(root)
    settemplabel.place(x=30, y=70)
    string_variable = StringVar()
    if y == 1:
        string_variable.set("Ustawiono temperaturę: " + x + " C    ")
        settemplabel.config(textvariable=string_variable)
    elif y == 2:
        string_variable.set("           Zamknięto zawór            ")
        settemplabel.config(textvariable=string_variable)
    elif y == 3:
        string_variable.set("            Otwarto zawór             ")
        settemplabel.config(textvariable=string_variable)


def checkSerialPort():
    if ser.isOpen() and ser.in_waiting:
        dataCanvas = Canvas(root)
        dataCanvas.place(x=220, y=25)
        recentPacket = ser.readline()
        recentPacketString = recentPacket.decode('utf')
        Label(dataCanvas, text=recentPacketString).pack(side='top', padx=0)


# creating tkinter window
root = Tk()
root.title('Regulator')

btn_Set = tk.Button(root, text="Ustaw", command=setTemp)
btn_Set.place(x=90, y=35)

btn_Off = tk.Button(root, text="Zamknij zawór", command=turnOffValve)
btn_Off.place(x=120, y=100)

btn_On = tk.Button(root, text="Otwórz zawór", command=turnOnValve)
btn_On.place(x=20, y=100)

entryTemp = Entry(root, width=3)
entryTemp.insert(0, "5")
entryTempLabel = tk.Label(root, text="Podaj temperaturę:")
entryTempLabel.place(x=30, y=10)
entryTemp.place(x=150, y=10)

root.geometry("400x150")

while True:
    root.update()
    checkSerialPort()

