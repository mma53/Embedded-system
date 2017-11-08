#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#define F_CPU 16E6
#include <util/delay.h>
#include "AVR_TTC_scheduler.h"
#include <Arduino.h>

volatile int extDist = 0;
volatile int lightInt = 0;
volatile int currTemp = 0;

void setupSerial() {
  Serial.begin(19200);
}

void checkExtDist() {
  /* Code to see extension distance here */
}

void checkTemp() {
  /* Code to check current temperature here */
}

void checkLight() {
  /* Code to check current light intensity here */
}

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
  ext = extDist;
  modifyValue(ext);
  extDist = ext;
  light = lightInt;
  for(i = 0; i < 3; i++) {
    modifyValue(light);
  }
  lightInt = light;
  temp = currTemp;
  for(i = 0; i < 2; i++) {
    modifyValue(temp);
  }
  currTemp = temp;
}

void tryToSendData() {
  modifyValues();
  Serial.write(extDist);
  Serial.write(lightInt);
  Serial.write(currTemp);
}

int main() {
  setupSerial();
  
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