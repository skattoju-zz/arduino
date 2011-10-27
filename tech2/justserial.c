#define F_CPU 1000000UL
#include <avr/io.h>
#include <string.h>
#include <stdlib.h>
#include <util/delay.h>
#include "led.h"

void serialstart();
void serialwrite(uint8_t);
void serialprint(char*);

int main() {
  init_leds();
  PORTD = 0b10;
  serialstart(2400);
  char* c = "hello";
  while (1) {

    turnOfftest();
    _delay_ms(100);
    serialwrite('a');
	serialprint(c);
    while ((UCSRA & 0b01000000) == 0b00000000);

    if ((UCSRA & 0b01000000) == 0b01000000) { turnOntest(); }
//     else {
//       turnOfftest();
//     }
  _delay_ms(100);

  }
  return 0;
}

void serialstart(){

  //  uint16_t i = (F_CPU/(32*baud))-1;
  //UBRRL = (unsigned char)(uint8_t)i;
  //UBRRH = (unsigned char)(uint8_t)(i >> 8);
  UBRRL = 25;
  //UBRRH = 0 << URSEL;
  //UBRRH = 0;
  UCSRA = 0 << 1;
  UCSRB |= (1 << RXEN) | (1 << TXEN); // enabled
  UCSRB = 0b11111000;

  UCSRC = 1 << URSEL;
  UCSRC = (1 << URSEL) | 0b10000110; // no parity, 8 data bits, 1 stop bits
}

void serialwrite(uint8_t c) {
  while ( !( UCSRA & (1<<UDRE)) );
  /* Put data into buffer, sends the data */
  UDR = (unsigned char)c;
}

void serialprint(char* ps) {
	serialwrite(ps[0]);
	int i = 0;
	while (ps[i] != '\0') {
		serialwrite(ps[i]);
		i++;
	}
}