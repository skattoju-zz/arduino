#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>

void blink1() {
  PORTD |= 0b00000001;
  _delay_ms(400);
  PORTD &= 0b11111110;
  _delay_ms(400);
  PORTD |= 0b00000001;
  _delay_ms(400);
  PORTD &= 0b11111110;
  _delay_ms(400);
  PORTD |= 0b00000001;
  _delay_ms(400);
  PORTD &= 0b11111110;
  _delay_ms(400);
}

void blink() {
  PORTD |= 0b00010001;
  _delay_ms(1000);
  PORTD &= 0b11101111;
  _delay_ms(1000);
}
