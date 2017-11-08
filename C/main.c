#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include <stdlib.h>
#include <stdint.h>
#define F_CPU 16E6
#include <util/delay.h>
#include "AVR_TTC_scheduler.h"

#define HIGH 0x1
#define LOW  0x0

volatile int extDist = 0;
volatile int lightInt = 0;
volatile int currTemp = 0;


/* Initialize serial connection with given baudrate. */
void initialize_serial(long baudrate) {
  UBRR0H = 0;
	UBRR0L = ((F_CPU / ( 16 * baudrate ))) - 1;
	UCSR0A = 0;
	UCSR0B = _BV(TXEN0) | _BV(RXEN0);
	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
}

/* Transmit 8-bit integer */
// wait for an empty transmit buffer
// UDRE is set when the transmit buffer is empty
void transmit(uint8_t data) {
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = data;
}

/* Receive bits until full byte has been set, then return bit */
char receive(void) {
	loop_until_bit_is_set(UCSR0A, RXC0);
	return UDR0;
}

uint8_t *intToBytes(int data) {
  uint8_t bytes[2];
  
  bytes[0] = (data >> 8) & 0xFF;
  bytes[1] = data & 0xFF;
  
  return bytes;
}

// ------------------------------------------------------------------------------------------------------

void checkExtDist() {
  /* Code to see extension distance here */
}

void checkTemp() {
  /* Code to check current temperature here */
}

void checkLight() {
  /* Code to check current light intensity here */
}

// ------------------------------------------------------------------------------------------------------

/* TO BE REMOVED!! Changes a given int by +1 or resets it to 0 when it has reached 10k */
int modifyValue(int value) {
  if(value > 10000) {
    value = 0;
    return value;
  }
  value++;
  return value;
}

/* TO BE REMOVED!! Modifies values of the global integers to provide testing input for the Python team */
void modifyValues() {
  int i;
  int ext = extDist;
  modifyValue(ext);
  extDist = ext;
  int light = lightInt;
  for(i = 0; i < 3; i++) {
    modifyValue(light);
  }
  lightInt = light;
  int temp = currTemp;
  for(i = 0; i < 2; i++) {
    modifyValue(temp);
  }
  currTemp = temp;
}

// ------------------------------------------------------------------------------------------------------


void sendData(int data) {
  uint8_t *toSend;
  toSend = intToBytes(data);
  transmit(toSend[0]);
  transmit(toSend[1]);
}

void tryToSendData() {
  modifyValues();
  sendData(extDist);
  sendData(lightInt);
  sendData(currTemp);
}

void sendK() {
	transmit(0x4B);
}

int main() {
  initialize_serial(19200);
  
  SCH_Init_T1();
  SCH_Start();
  
  
  
  // 10ms/tick, 1s = 100.
  SCH_Add_Task(checkExtDist, 0, 500); // Check screen extension every 5 seconds.
  SCH_Add_Task(checkTemp, 0, 4000); // Check temperature every 40 seconds.
  SCH_Add_Task(checkLight, 0, 3000); // Check light intensity every 30 seconds.
  SCH_Add_Task(tryToSendData, 0, 6000); // Try to send data every 60 seconds.
  SCH_Add_Task(sendK, 1000, 500);
  
  while(1) {
    SCH_Dispatch_Tasks();
  }
}
