//source of PulseIn translation: https://forum.arduino.cc/index.php?topic=356243.0
#include <avr/io.h>

unsigned long RC_VALUE;

void pulseIn()
{
	char RC_PIN_STATE = (PIND & (1<<PD2));
	
	//timeout zone
	unsigned long numloops = 0;
	unsigned long maxloops = 500000;
	unsigned long width = 0;
	// wait for any previous pulse to end
	while ( RC_PIN_STATE == 1)
	{
		RC_PIN_STATE = (PIND & (1<<PD2)); //keep reading the pin until it changes the state
		if (numloops++ == maxloops) break;
	}
	// wait for the pulse to start
	while (RC_PIN_STATE == 0)
	{
		RC_PIN_STATE = (PIND & (1<<PD2)); //keep reading the pin until it changes the state
		if (numloops++ == maxloops) break;
	}
	
	// wait for the pulse to stop @ here we are measuring the pulse width = incrementing the WIDTH value by one each cycle. atmega328 1 micro second is equal to 16 cycles.
	while (RC_PIN_STATE == 1)
	{
		RC_PIN_STATE = (PIND & (1<<PD2));
		if (numloops++ == maxloops) break;
		width++;
	}

	RC_VALUE = width/16;
}

int main(void)
{
	DDRD = (0<<PD2); // port PD2 is an input for the RC signal
	DDRD = (1<<PD3); // port is and output for LED
	
	//char RC_PIN_STATE = (PIND & (1<<PD2)); // read the state of pin PD2 and put the state into variable
	
    while(1)
    {
      pulseIn();
	  if(RC_VALUE > 1000 && RC_VALUE < 1395)
	  {
		  PORTD = (1<<PD3); // turn LED on
	  }
		
    	else if(RC_VALUE > 1600 && RC_VALUE < 2200)
		{
		PORTD = (0<<PD3); // turn LED off
		}	
    }
}