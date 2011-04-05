#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main(){

/*
	DDRB |= 0b00001110; // Set pins 15, 16, 17 for output (PB1, PB2, PB3)
	DDRD |= 0b11111111; // Set pins 5, 11, 12 for output (PD3, PD5, PD6)
	DDRC |= 0b00000010;
*/
	// PORTC |= 0b00000010;
	// PORTB |= 0b00001100;
	// PORTD |= 0b10011111;

	DDRB |= 0b00100000;

	while (1) {

		PORTB |= 0b00100000;
		_delay_ms(500);
		PORTB &= 0b11011111;
		_delay_ms(500);
	}

return 0;

}

