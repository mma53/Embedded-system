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

volatile int extDist = 0x26;
volatile int lightInt = 0x26;
volatile int currTemp = 0x26;

union {
  int intValue;
  char charArray[2];
} intToChar;

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

void transmitInt(int data) {
  int i;
  intToChar.intValue = data;\
  for(i = 0; i < 2; i++) {
    char toSend = intToChar.charArray[i];
    transmit(toSend);
  }
}

/* Receive bits until full byte has been set, then return bit */
char receive(void) {
	loop_until_bit_is_set(UCSR0A, RXC0);
	return UDR0;
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

// ------------------------------------------------------------------------------------------------------

void checkExtDist() {
  /* Code to see extension distance here */
  
  extDist = modifyValue(extDist);
  
  return;
}

void checkTemp() {
  /* Code to check current temperature here */
  
  currTemp = modifyValue(currTemp);
  
  return;
}

void checkLight() {
  /* Code to check current light intensity here */
  
  lightInt = modifyValue(lightInt);
  
  return;
}

// ------------------------------------------------------------------------------------------------------

void sendEOL() {
  transmitInt(0x0D0A);
}

void sendData() {
  transmitInt(extDist);
  transmitInt(lightInt);
  transmitInt(currTemp);
  sendEOL();
}



int main() {
  initialize_serial(19200);
  
  SCH_Init_T1();
  SCH_Start();
  
  
  // 10ms/tick, 1s = 100.
  SCH_Add_Task(checkExtDist, 1, 500); // Check screen extension every 5 seconds.
  SCH_Add_Task(checkTemp, 2, 4000); // Check temperature every 40 seconds.
  SCH_Add_Task(checkLight, 3, 3000); // Check light intensity every 30 seconds.
  SCH_Add_Task(sendData, 5, 6000); // Try to send data every 60 seconds.
  
  while(1) {
    SCH_Dispatch_Tasks();
  }
}
