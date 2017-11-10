import serial

# basic code, taken from GitHub.

ser = serial.Serial()
ser.baudrate = 19200
ser.port = 'COM4'
ser.open()

print("connected to: " + ser.portstr)
count = 1

while True:
    line = ser.readline()
    print('Line ' + str(count) + ':')
    print('  1: ' + str(line[0:2]))
    print('   int: ' + int(line[0:2]))
    print('  2: ' + str(line[2:4]))
    print('  3: ' + str(line[4:6]))
    print('    ' + str(line[5]))

ser.close()
