import serial

# basic code, taken from GitHub.

ser = serial.Serial()
ser.baudrate = 19200
ser.port = 'COM3'
ser.open()

print("connected to: " + ser.portstr)
count = 1

while True:
    line = ser.readline
    print('Length of line ' + count + str(': ') + len(line))
    print(' 1: ' + int(line[0:2]))
    print(' 2: ' + int(line[2:4]))
    print(' 3: ' + int(line[4:6]))

ser.close()
