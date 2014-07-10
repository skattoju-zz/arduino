#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include "pwm.h"

/*
Modified by Siddhartha Kattoju, March 2011.
based on https://www.mainframe.cx/~ckuethe/avr-c-tutorial/lesson7b.c
*/

static int speed_old = 0;
		static uint16_t pwm = 0;

void setup_pwm(){
	// pin PB1 / pin 15
	DDRB |= 1 << 1;
	DDRB |= 1 << 0;

	/* Waveform Generation Mode 3 - Fast PWM */
	TCCR1A |= _BV(WGM10) | _BV(WGM12);

	TIMSK |= _BV(OCIE1A) | _BV(TOIE1);
	/*
	 * Compare Output Mode - fast PWM
	 * Non-inverting mode drives the output high while the counter
	 * is greater than OCRNx. Inverting mode drives the output low
	 * while the counter is greater than OCRNx.
	 */

	TCCR1A |= _BV(COM1A1) | _BV(COM1A0); /* inverting: fade down */
	// TCCR1A |= _BV(COM1B1); 		     /* non-inverting: fade up */

	/* reset all the timers and comparators */
	OCR1A = 0;
	TCNT1 = 0;

	/*
	 * Clock Source 1 - CLK. Setting this bit late allows us to
	 * initialize the registers before the clocks start ticking
	 */
	TCCR1B |= _BV(CS00);
	TCCR1B &= 0b11111001;

	/*
	 * Arduino pins 5 & 6 (PORTD.5 and PORTD.6) are PWM driven by TIMER0
	 * "The setup of the OC0x should be performed before setting the Data
	 * Direction Register for the port pin to output." -- S14.5.3
	 */

	DDRB |= _BV(PB1);

}

void update_led_brightness(int speed){

		/*
		 * slowly crank up the compare register. since one output
		 * is inverting, the net result is to fade from one channel
		 * to the other.
		 */
		//OCR0A++; //fade up.
		//OCR0B++; //fade down.



		if(speed > speed_old){

		    pwm = (pwm == 0x03FF ? 0x03FF : pwm+5);
		}
		else if(speed < speed_old){

		    pwm = (pwm == 0x0000 ? 0x0000 : pwm-5);
    		}

		unsigned char sreg;
		//unsigned int i;
		/* Save Global Interrupt Flag */
		sreg = SREG;
		/* Disable interrupts */
		cli();
		/* Set TCNT1 to i */
		OCR1A = pwm;
		/* Restore Global Interrupt Flag */
		SREG = sreg;

		speed_old = speed;
}
