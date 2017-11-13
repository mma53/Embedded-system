#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <stdlib.h>
#include <stdint.h>
#include "arduino_basic_serial.h"

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
    char toSend = intToChar.charArray[1-i];
    transmit(toSend);
  }
}

/* Receive bits until full byte has been set, then return bit */
char receive(void) {
	loop_until_bit_is_set(UCSR0A, RXC0);
	return UDR0;
}