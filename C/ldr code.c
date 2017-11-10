#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>       // needed for uint8_t

/**
This amazing thing is from https://arduino.stackexchange.com/a/11770
**/
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

int main (void)
{
  /* Set all pins of PORTB for output. */
  DDRB = DDRB | 0b11111111;
  /* Set pin 0 of PORTC for input. */
  DDRC &= _BV(DDC0);
  int photo, ledval;
  
  while(1) {
    photo = ADCsingleREAD(0);

    if (photo < 20) {
      ledval = 1;
      } else {
      ledval = 0;
    }
    /* Disable all LEDS */
    PORTB &= 0x0;
    /** Enable measured bitmask */
    PORTB |= ledval;
  }
  
  return 0;
}