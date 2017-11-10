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

volatile int extDist = 0x0026;
volatile int lightInt = 0x0026;
volatile int currTemp = 0x0026;


// ------------------------------------------------------------------------------------------------------
/* AUXILIARY FUNCTIONS FOR CHECKEXTDIST */


// ------------------------------------------------------------------------------------------------------
/* AUXILIARY FUNCTIONS FOR CHECKTEMP */


// ------------------------------------------------------------------------------------------------------
/* AUXILIARY FUNCTIONS FOR CHECKLIGHT */


// ------------------------------------------------------------------------------------------------------

void checkExtDist() {
  /* Code to see extension distance here */
  (extDist >= 10000) ? (extDist = 0) : (extDist += 1);
  
  return;
}

void checkTemp() {
  /* Code to check current temperature here */
  (currTemp >= 10000) ? (currTemp = 0) : (currTemp += 3);
  
  return;
}

void checkLight() {
  /* Code to check current light intensity here */
  (lightInt >= 10000) ? (lightInt = 0) : (lightInt += 2);
  
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
