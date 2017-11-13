import serial
from time import sleep

# basic code, taken from GitHub.


def inputToInt(input):
    toReturn = input[0]*256 + input[1]
    return toReturn

def strToInt(input):
    toReturn = int(input);
    return toReturn

def intToOutput(input):
    output = input.to_bytes(2, byteorder='big')
    return output;

def initializeSensors(ser):
    #TODO: Only use this function when settings are changed.
    sleep(2)
    ser.write(0x01.to_bytes(1, 'big'))
    ser.write(0x00.to_bytes(1, 'big'))
    ser.write(0x01.to_bytes(1, 'big'))
    ser.write(0x02.to_bytes(1, 'big'))

ser = serial.Serial()
ser.baudrate = 19200
ser.port = 'COM3'
ser.open()

print("connected to: " + ser.portstr)

initializeSensors(ser)

while True:
    line = ser.readline()
    print('Data received:')
    currDist = inputToInt(line[0:2])
    currLight = inputToInt(line[2:4])
    currTemp = inputToInt(line[4:6])
    print('  curr. Dist : ' + str(currDist) + ' / ' + str(strToInt(line[0])) + ' ' + str(strToInt(line[1])))
    print('  curr. Light: ' + str(currLight) + ' / ' + str(strToInt(line[2])) + ' ' + str(strToInt(line[3])))
    print('  curr. Temp : ' + str(currTemp) + ' / ' + str(strToInt(line[4])) + ' ' + str(strToInt(line[5])))

ser.close()
