#define F_CPU 1000000UL
#include <avr/io.h>
#include <string.h>
#include <stdlib.h>
#include <util/delay.h>
#include "led.h"

void serialstart(int);
void serialwrite(uint8_t);

int main() {
  init_leds();
  PORTD = 0b10;
  serialstart(2400);
  
  while (1) {
    
    turnOfftest();
    _delay_ms(1500);
    serialwrite('a');
    while ((UCSRA & 0b01000000) == 0b00000000);
    
    if ((UCSRA & 0b01000000) == 0b01000000) { turnOntest(); }
//     else {
//       turnOfftest();
//     }
  _delay_ms(1500);
    
  }
  return 0;
}

void serialstart(int baud){

  //  uint16_t i = (F_CPU/(32*baud))-1;
  //UBRRL = (unsigned char)(uint8_t)i;
  //UBRRH = (unsigned char)(uint8_t)(i >> 8);
  UBRRL = 25;
  UBRRH = 0 << URSEL;
  UBRRH = 0;
  UCSRA = 0b0000;
  UCSRB |= (1 << RXEN) | (1 << TXEN); // enabled
  UCSRB = 0b11111000;

  UCSRC = 1 << URSEL;
  UCSRC = (1 << URSEL) | 0b10000110; // even parity, 8 data bits, 2 stop bits  
}

void serialwrite(uint8_t c) {
  while ( !( UCSRA & (1<<UDRE)) );
  /* Put data into buffer, sends the data */
  UDR = (unsigned char)c;
}