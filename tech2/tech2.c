#define F_CPU 1000000UL  // 1 MHz
#define BAUD_RATE 4800   //serial comm baud rate

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include "led.h"
#include "serial.h"
#include "sensors.h"

int inc = 0;
volatile int enable_B2 = 0b00000000;

int main() {
// setting up external interrupt to trigger at logical change
      MCUCR |= 0<<1;
      MCUCR |= 1<<0;

//enable external interrupt
        GICR |= 0b1 << 6;

// global interrupts enable
	sei();

// setting up timer interrupt
        TCCR1A = 0b0000000;
	TCCR1B = 0b0001010;

// setting up port C for LED output
      	DDRC |= 0b111;
      	PORTC |= 0b111;

//setting up compare register for timer interrupt
	OCR1A = 0xA200;
	//OCR1A = 0x0002;

// setting up port D for input
	DDRD = 0b0000;

// setting up serial communication 8N1 asynchronous mode.
	serial_init(( F_CPU / BAUD_RATE / 16UL ) - 1);

// initial blink
       turnOnBothLeds();
       _delay_ms(300);
       turnOffBothLeds();

// main loop that polls B2
     while(1){

            if (((PIND & 0b00001000) == 0b00000000)) {
			_delay_ms(50);
			while (((PIND & 0b00001000) == 0b00000000)) {
					_delay_ms(1);
				}
		//an object was detected
		serial_print("object detected");
            }
       }
	return 0;
}

// interrupt service routine for B1 clicks
ISR(INT0_vect) {

	// Delay (for debouncing)
	_delay_ms(50);

	// if pin is high, there is an object
	if ((PIND & 0b00000100) == 0b00000100) {
		// so turn on an led
		turnOnLed1();
	}
	// if pin is low, there is no object
	else {
		// so turn off led
		turnOffLed1();
	}

	// Clear the external interrupt flag.
	// Used for debouncing, cancels any
	  // interrupt that happened during the
	  // debounce period.
	  GIFR = 1 << 6;
}

//timer interrupt service routine : make's LEDs alternate at 3Hz
ISR(TIMER1_COMPA_vect) {

	// if two lights have the same value
	if ((PORTC & 0b01) == (PORTC & 0b10)){
		// XOR them with 01
		PORTC = PORTC ^ 0b01;
		}

	// otherwise, invert the values
	PORTC = PORTC ^ 0b11;
	//TCNT1 = 0xAF03;
	//TCNT1 = 0xA2C2;
}
