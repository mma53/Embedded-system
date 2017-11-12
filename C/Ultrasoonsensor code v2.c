//oude v2
#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16E6
#include <util/delay.h>

#define HIGH 0x1
#define LOW 0x0

union {
  int intValue;
  char charArray[2];
} intToChar;

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

//source of PulseIn translation: https://forum.arduino.cc/index.php?topic=356243.0
unsigned long RC_VALUE;

unsigned long pulseIn()
{
  char RC_PIN_STATE = (PIND & (1<<PD3));
  
  //timeout zone
  unsigned long numloops = 0;
  unsigned long maxloops = 500000;
  unsigned long width = 0;
  // wait for any previous pulse to end
  while ( RC_PIN_STATE == 1)
  {
    RC_PIN_STATE = (PIND & (1<<PD3)); //keep reading the pin until it changes the state
    if (numloops++ == maxloops) break;
  }
  // wait for the pulse to start
  while (RC_PIN_STATE == 0)
  {
    RC_PIN_STATE = (PIND & (1<<PD3)); //keep reading the pin until it changes the state
    if (numloops++ == maxloops) break;
  }
  
  // wait for the pulse to stop @ here we are measuring the pulse width = incrementing the WIDTH value by one each cycle. atmega328 1 micro second is equal to 16 cycles.
  while (RC_PIN_STATE == 1)
  {
    RC_PIN_STATE = (PIND & (1<<PD3));
    if (numloops++ == maxloops) break;
    width++;
  }

  RC_VALUE = width/16;

  return RC_VALUE;
}
// void loop()
// {
//   // const int trigPin = 2; // pind2
//   // const int echoPin = 4; // pind4
//   // establish variables for duration of the ping,
//   // and the distance result in inches and centimeters:
//   long duration, cm;
//   // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
//   // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
//   pinMode(trigPin, OUTPUT); //pind2 is output
//   digitalWrite(trigPin, LOW);
//   delayMicroseconds(2); //__delay__us(2)
//   digitalWrite(trigPin, HIGH);
//   delayMicroseconds(10); //__delay__us(10)
//   digitalWrite(trigPin, LOW);
//   // Read the signal from the sensor: a HIGH pulse whose
//   // duration is the time (in microseconds) from the sending
//   // of the ping to the reception of its echo off of an object.
//   pinMode(echoPin, INPUT);
//   duration = pulseIn(echoPin, HIGH);
//   // convert the time into a distance
//   cm = microsecondsToCentimeters(duration);
//   Serial.print("distancia ");
//   Serial.print(cm);
//   Serial.print(" cm");
//   Serial.println();
//   delay(100);
// }

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

int main()
{
  /* Set baudrate. */
  initialize_serial(9600);
  /* initialize some variables */
  long duration, cm;
  /* Set pind2 to input and pind4 to output*/
  DDRD = DDRD | 0B00001000; // port PD2 is an input for the RC signal
  while (1)
  {
    // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
    // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
    PORTD &=~ _BV(PB3);
    _delay_us(2);
    PORTD |= _BV(PB3);
    _delay_us(10);
    PORTD &=~ _BV(PB3);
    duration = pulseIn(RC_VALUE);   //nog vertalen naar atmel
    cm = microsecondsToCentimeters(duration);
    Serial.print(cm);
    Serial.print('\n');
    _delay_ms(100);
  }
}