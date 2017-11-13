#include <avr/io.h>
#include <avr/eeprom.h>
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

struct analogSensor {
  char enabled;
  char port;
  char extVal;
  char retVal;
};

struct analogSensor lightSensor = {0,0,150,120};

struct analogSensor tempSensor = {0,0,155,148};

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
  if(tempSensor.enabled == 0) {
    return;  
  }
  currTemp = ADCsingleREAD(tempSensor.port);
  if(lightSensor.enabled == 0) {
	  if(currTemp > tempSensor.extVal && extDist != 2) {
      extendScreen();
    }
    if(currTemp < tempSensor.retVal && extDist != 0) {
      retractScreen();
    }
  }
  return;
}

void checkLight() {
  if(lightSensor.enabled == 0) {
    return;
  }
  lightInt = ADCsingleREAD(lightSensor.port);
  if(lightInt >= lightSensor.extVal && extDist != 2) {
    extendScreen();
  }
  if(lightInt < lightSensor.retVal && extDist != 0) {
    retractScreen();
  }
  return;
}

// ------------------------------------------------------------------------------------------------------

/* Receive bits until full byte has been set OR until timeout has occured, then return bit */
char receive2(void) {
  long i = 0;
  do{
    if(i >= 160000) {
      return 0x00;
    }
    i++;
  }while(bit_is_clear(UCSR0A, RXC0));
	return UDR0;
}

void sendEOL() {
  transmitInt(0x0D0A);
}

void sendData() {
  transmitInt(extDist);
  transmitInt(lightInt);
  transmitInt(currTemp);
  sendEOL();
}

void valToLEDs(uint8_t value) {
  PORTB &=~ _BV(PD0);
  PORTB &=~ _BV(PD1);
  PORTB &=~ _BV(PD2);
  if(value >= 4) {
    PORTB |= _BV(PD2);
    value -= 4;
  }
  if(value >= 2) {
    PORTB |= _BV(PD1);
    value -= 2;
  }
  if(value >= 1) {
    PORTB |= _BV(PD0);
    value -= 1;
  }
}

void registerSensors() {
  /* TODO: Update from settings stored in EEPROM.
   * IMPORTANT: ONLY update EEPROM when settings are changed!
   */
  int i;
  for(i=1;i<9;i++) {
    eeprom_update_byte((uint8_t*)i,receive());
  }
}

void checkSensors() {
  if(receive2() == 0xFF) {
    registerSensors();
  }
  lightSensor.enabled = eeprom_read_byte((uint8_t*)1);
  lightSensor.port = eeprom_read_byte((uint8_t*)2);
  lightSensor.extVal = eeprom_read_byte((uint8_t*)3);
  lightSensor.retVal = eeprom_read_byte((uint8_t*)4);
  tempSensor.enabled = eeprom_read_byte((uint8_t*)5);
  tempSensor.port = eeprom_read_byte((uint8_t*)6);
  tempSensor.extVal = eeprom_read_byte((uint8_t*)7);
  tempSensor.retVal = eeprom_read_byte((uint8_t*)8);
}

int main() {
  initialize_serial(19200);
  initializePortB(SEND);
  PORTB |= _BV(PD0);
  PORTB |= _BV(PD1);
  PORTB |= _BV(PD2);
  _delay_ms(100);
  checkSensors();
  PORTB &=~ _BV(PD1);
  PORTB &=~ _BV(PD2);
  
  SCH_Init_T1();
  SCH_Start();
  
  
  
  // 10ms/tick, 1s = 100 ticks.
  SCH_Add_Task(checkExtDist, 1, 500); // Check screen extension every 5 seconds.
  SCH_Add_Task(checkTemp, 2, 4000); // Check temperature every 40 seconds.
  SCH_Add_Task(checkLight, 3, 3000); // Check light intensity every 30 seconds.
  SCH_Add_Task(sendData, 5, 6000); // Try to send data every 60 seconds.
  SCH_Add_Task(switchB1,6, 50); // Blink if applicable, every second.
  
  
  while(1) {
    SCH_Dispatch_Tasks();
  }
}
