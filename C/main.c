#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#define F_CPU 16E6
#include <util/delay.h>
#include "AVR_TTC_scheduler.h"

#define HIGH 0x1
#define LOW  0x0

volatile int extDist = 0;
volatile int lightInt = 0;
volatile int currTemp = 0;

void checkExtDist() {
  /* Code to see extension distance here */
}

void checkTemp() {
  /* Code to check current temperature here */
}

void checkLight() {
  /* Code to check current light intensity here */
}

void tryToSendData() {
  /* Code to send collected data to central control */
}

int main() {
  SCH_Init_T1();
  SCH_Start();
  
  // 10ms/tick, 1s = 100.
  SCH_Add_Task(checkExtDist, 0, 500); // Check screen extension every 5 seconds.
  SCH_Add_Task(checkTemp, 0, 4000); // Check temperature every 40 seconds.
  SCH_Add_Task(checkLight, 0, 3000); // Check light intensity every 30 seconds.
  SCH_Add_Task(tryToSendData, 0, 6000); // Try to send data every 60 seconds.
  
  while(1) {
    SCH_Dispatch_Tasks();
  }
}