import serial

# basic code, taken from GitHub.

ser = serial.Serial()
ser.baudrate = 19200
ser.port = 'COM3'
ser.open()

print("connected to: " + ser.portstr)

def inputToInt(input):
    toReturn = input[1] * 255 + input[0]
    return toReturn

while True:
    line = ser.readline()
    print('Data received:')
    currDist = inputToInt(line[0:2])
    currLight = inputToInt(line[2:4])
    currTemp = inputToInt(line[4:6])
    print('  curr. Dist : ' + str(line[0:2]) + ' / ' + str(currDist))
    print('  curr. Light: ' + str(line[2:4]) + ' / ' + str(currLight))
    print('  curr. Temp : ' + str(line[4:6]) + ' / ' + str(currTemp))

ser.close()
