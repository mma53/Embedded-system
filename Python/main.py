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

def inputToInt(input):
    toReturn = input[0]*255 + input[1]
    return toReturn

while True:
    line = ser.readline()
    print('Data received:')
    currDist = inputToInt(line[0:2])
    currLight = inputToInt(line[2:4])
    currTemp = inputToInt(line[4:6])
    print('  curr. Dist : ' + str(currDist))
    print('  curr. Light: ' + str(currLight))
    print('  curr. Temp : ' + str(currTemp))

ser.close()
