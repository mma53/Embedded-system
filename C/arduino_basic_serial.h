#ifndef arduino_basic_serial_h_
#define arduino_basic_serial_h_
#define F_CPU 16E6

union {
  int intValue;
  char charArray[2];
} intToChar;

// Function prototypes
void initialize_serial(long baudrate);
void transmit(uint8_t data);
void transmitInt(int data);
char receive(void);

#endif