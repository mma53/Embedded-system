#include <avr/eeprom.h>
#include <stdint.h>
#include <stdlib.h>

int main() {
  int i;
  for(i = 1; i < 9; i++) {
    eeprom_update_byte((uint8_t*)i,0x00);
  }
}