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

int main() {
  SCH_Init_T1();
  SCH_Start();
  
  // 10ms/tick, 1s = 100.
  SCH_Add_Task(checkExtDist, 0, 500);
  SCH_Add_Task(checkTemp, 0, 4000);
  SCH_Add_Task(checkLight, 0, 3000);
  
  while(1) {
    SCH_Dispatch_Tasks();
  }
}