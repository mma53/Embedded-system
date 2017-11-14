import serial

ser = serial.Serial(port='COM4', baudrate=9600, bytesize=serial.EIGHTBITS, parity=serial.PARITY_NONE, timeout=1)

try:
    ser.isOpen()
    print("Serial port is open")
except:
    print("Serial port is closed")
    exit()

if(ser.isOpen()):
    try:
        while(1):
            print(ser.read(size=2))
    except Exception:
        print("Error")
else:
    print("Cannot open serial port")