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
    ser.write(0x0F.to_bytes(1, 'big')) # opening
    ser.write(0x01.to_bytes(1, 'big')) # ENABLED
    ser.write(0x00.to_bytes(1, 'big')) # PORT
    ser.write(0x96.to_bytes(1, 'big')) # EXTVAL 150 = 0x96 // 0x32 = 50
    ser.write(0x78.to_bytes(1, 'big')) # RETVAL 120 = 0x78 // 0x28 = 40
    ser.write(0x01.to_bytes(1, 'big')) # ENABLED 0x01
    ser.write(0x01.to_bytes(1, 'big')) # PORT 0x01
    ser.write(0x9B.to_bytes(1, 'big')) # EXTVAL 0x9B
    ser.write(0x94.to_bytes(1, 'big')) # RETVAL 0x94

    # NOTE: NEED TO SEND TWO BYTES FOR LIGHT SENSOR!

ser = serial.Serial()
ser.baudrate = 19200
ser.port = 'COM4'
ser.open()

print("connected to: " + ser.portstr)

initializeSensors(ser)

while True:
    line = ser.readline()
    print('Data received:')
    if len(line) == 8:
        currDist = inputToInt(line[0:2])
        currLight = inputToInt(line[2:4])
        currTemp = inputToInt(line[4:6])
        print('  curr. Dist : ' + str(currDist) + ' / ' + str(strToInt(line[0])) + ' ' + str(strToInt(line[1])))
        print('  curr. Light: ' + str(currLight) + ' / ' + str(strToInt(line[2])) + ' ' + str(strToInt(line[3])))
        print('  curr. Temp : ' + str(currTemp) + ' / ' + str(strToInt(line[4])) + ' ' + str(strToInt(line[5])))

ser.close()
