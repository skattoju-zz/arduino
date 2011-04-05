#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#define F_CPU 1000000UL
// LED enable disable
// remember, it's active LOW

void init_leds(){

// setting up port B for LED output
        DDRB |= 0b111;
        PORTB |= 0b111;
	DDRD |= 1 << 7;
	PORTD |= 1 << 7;
}

void turnOnLed1() {
       PORTB &= 0b11111110;
}

void turnOnLed2() {
       PORTB &= 0b11111101;
}

void turnOntest() {

	PORTD &= 0b01111111;
}

void turnOffLed1() {
       PORTB |= 0b00000001;
}

void turnOfftest() {

        PORTD |= 0b10000000;
}


void turnOffLed2() {
       PORTB |= 0b00000010;
}

void turnOnBothLeds() {
       PORTB = 0b00 << 0;
}

void turnOffBothLeds() {
       PORTB = 0b11 << 0;
}

void blink() {

	turnOntest();
	_delay_ms(500);
	turnOfftest();
	_delay_ms(500);
}

void startblink() {

	turnOntest();
	_delay_ms(200);
	turnOfftest();
	_delay_ms(200);
	turnOntest();
	_delay_ms(200);
	turnOfftest();
	_delay_ms(200);
}
