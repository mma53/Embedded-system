#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include <stdlib.h>
#include <stdint.h>
#define F_CPU 16E6
#include <util/delay.h>
#include "AVR_TTC_scheduler.h"
#include "arduino_basic_serial.h"

#define HIGH 0x1
#define LOW  0x0

#define READ 0x0
#define SEND 0x1

char sensorType = 'L';

volatile int extDist = 0;
volatile int lightInt = 0;
volatile int currTemp = 0;
int lightOn = 0;

// ------------------------------------------------------------------------------------------------------
/* AUXILIARY FUNCTION FOR CHECKLIGHT // CHECKTEMP */

/*
 * This amazing thing is from https://arduino.stackexchange.com/a/11770
 * Reads from a single pin on the analog port.
 * adctouse = pin to read from. 
 */
int ADCsingleREAD(uint8_t adctouse)
{
  int ADCval;

  ADMUX = adctouse;        // use #1 ADC
  ADMUX |= (1 << REFS0);   // use AVcc as the reference
  ADMUX &= ~(1 << ADLAR);  // clear for 10 bit resolution

  // 128 prescale for 8Mhz
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

  ADCSRA |= (1 << ADEN);    // Enable the ADC
  ADCSRA |= (1 << ADSC);    // Start the ADC conversion

  while(ADCSRA & (1 << ADSC)); // waits for the ADC to finish

  ADCval = ADCL;
  ADCval = (ADCH << 8) + ADCval; // ADCH is read so ADC can be updated again

  return ADCval;
}

// ------------------------------------------------------------------------------------------------------

void initializePortB(uint8_t state) {
  uint8_t bitmask;
  (state == 0) ? (bitmask = 0b000000) : (bitmask = 0b111111);
  DDRB |= bitmask;
  return;
}

// ------------------------------------------------------------------------------------------------------

void extendScreen() {
  PORTB &=~ _BV(PD0);
  PORTB |= _BV(PD1);
  PORTB |= _BV(PD2);
  extDist = 1;
  return;
}

void retractScreen() {
  PORTB |= _BV(PD0);
  PORTB |= _BV(PD1);
  PORTB &=~ _BV(PD2);
  extDist = 1;
  return;
}

void switchB1() {
  if(extDist == 1) {
    if(lightOn == 1) {
      PORTB &=~ _BV(PD1);
      lightOn = 0;
    } else {
      PORTB |= _BV(PD1);
      lightOn = 1;
    }
  } else {
    PORTB &=~ _BV(PD1);
  }
  return;
}

// ------------------------------------------------------------------------------------------------------

void checkExtDist() {
  // TESTING CODE. TO BE REPLACED BY MEASUREMENTS.
  if(extDist == 1 && PORTB <= 3) {
	  extDist = 0;
  }
  if(extDist == 1 && PORTB >= 4){
    extDist = 2;
  }
  
  return;
}

void checkTemp() {
  currTemp = ADCsingleREAD(1);
  if(currTemp > 155 && extDist != 2) {
    extendScreen();
  }
  if(currTemp < 148 && extDist != 0) {
    retractScreen();
  }
  return;
}

void checkLight() {
  lightInt = ADCsingleREAD(0);
  if(lightInt >= 150 && extDist != 2) {
    extendScreen();
  }
  if(lightInt < 120 && extDist != 0) {
    retractScreen();
  }
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
  initializePortB(SEND);
  PORTB |= _BV(PD0);
  
  SCH_Init_T1();
  SCH_Start();
  
  
  
  // 10ms/tick, 1s = 100 ticks.
  if(sensorType == 'L') {
  SCH_Add_Task(checkLight, 3, 3000); // Check light intensity every 30 seconds.
  }
  if(sensorType == 'T') {
    SCH_Add_Task(checkTemp, 2, 4000); // Check temperature every 40 seconds.
  }
  
  SCH_Add_Task(checkExtDist, 1, 500); // Check screen extension every 5 seconds.
  SCH_Add_Task(sendData, 5, 6000); // Try to send data every 60 seconds.
  SCH_Add_Task(switchB1,6, 50); // Blink if applicable, every second.
  
  
  while(1) {
    SCH_Dispatch_Tasks();
  }
}
