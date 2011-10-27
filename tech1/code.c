#include <avr/io.h>
#include <stdio.h>
#include <util/delay_basic.h>
#include <avr/interrupt.h>
#define F_CPU 1000000UL  // 1 MHz
#include <util/delay.h>

// LED stuff
// remember, it's active LOW

void turnOnLed1() {
	PORTC &= 0b11111110;
}

void turnOnLed2() {
	PORTC &= 0b11111101;
}

void turnOffLed1() {
	PORTC |= 0b00000001;
}

void turnOffLed2() {
	PORTC |= 0b00000010;
}

void turnOnBothLeds() {
	PORTC &= 0b11111100;
}

void turnOffBothLeds() {
	PORTC |= 0b00000011;
}

int inc = 0;

void main() {
       sei();
// setting up external interrupt to trigger at falling edge
       MCUCR |= 1<<1;
       MCUCR |= 1<<0;

// setting up timer interrupt
       TCCR0 = 0b111;

// setting up port C for LED output
       DDRC |= 0b11;
	PORTC |= 0b11;

// setting up port D for input
	DDRD = 0b0000;
// might be needed:
	PORTD = 0b1100;

// initial blink
	turnOnBothLeds();
        //_delay_ms(700);
	turnOffBothLeds();

// main loop that polls B2
       while (1) {
		// Or PIND with
			PORTB = PIND | 0b11110111;
               if ((PIND | 0b11110111) == 0b11110111) {
                       //_delay_ms(256);
                       while ((PIND | 0b11110111) == 0b11110111) {
                       }
                       TIMSK = ~TIMSK;
               }
       }
}

// interrupt service routine for B1 clicks
ISR(INT0_vect) {
       switch (inc) {

	// start alternation of LEDs
       case 0:
               TIMSK = 1;
               turnOnLed1();
               break;
	// turn LEDs on
       case 1:
		turnOnBothLeds();
               break;
	// turn LEDs off
       case 2:
               turnOffBothLeds();
               break;
	// turn LEDs on
       case 3:
               turnOnBothLeds();
               break;
	// LED1 off, LED2 on
       case 4:
               turnOffLed1();
		turnOnLed2();
		// disable external interrupt?
               GICR |= 1<<6;
               break;

       default:
               _delay_ms(1);
       }
       inc++;
}

//timer interrupt service routine : makeas LEDs alternate every 5/11s
ISR(TIMER0_OVF_vect ) {
       PORTC = ~PORTC & 0b00000011;
}


