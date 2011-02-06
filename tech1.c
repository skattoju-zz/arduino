#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 1000000UL  // 1 MHz

#include <util/delay.h>

//#define F_CPU 1000000UL  // 1 MHz

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
       PORTC = 0b00 << 0;
}

void turnOffBothLeds() {
       PORTC = 0b11 << 0;
}

int inc = 0;
volatile int enable_B2 = 0b00000000;

int main() {

int poll = 0;
// setting up external interrupt to trigger at falling edge
      MCUCR |= 0<<1;
      MCUCR |= 1<<0;
	  //enable external interrupt
      GICR |= 0b1 << 6;

sei();
// setting up timer interrupt
          //TIMSK |= 1 << 0;
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

// initial blink
       turnOnBothLeds();
       _delay_ms(300);
       turnOffBothLeds();

// main loop that polls B2
     while(1){
	    //poll = (PIND >> 3) & 0b1;
            if (((PIND & 0b00001000) == 0b00000000)) {
			_delay_ms(100);
			while (((PIND & 0b00001000) == 0b00000000)) {
					_delay_ms(1);
				}
                //toggle timer interrupts
                //TCNT1 = 0x01;
			TIMSK = TIMSK ^ (1 << 4);
			_delay_ms(100);

            }
       }


	return 0;
}

// interrupt service routine for B1 clicks
ISR(INT0_vect) {
	// Delay (for debouncing)
	_delay_ms(50);

	// if pin is high (on release)
	if ((PIND & 0b00000100) == 0b00000100) {
		// disable timer0 interrupt
		TIMSK = 0 << 4;

		switch (inc) {
		// First click
		// start alternation of LEDs
			case 0:
				// accept clicks from B2
				//enable_B2 = 1;
				if (TIMSK & (1 << 4) != 0b00010000) {
				turnOnLed2();
				}
				// enable timer0 interrupt
				TIMSK = 1<<4;
				// reset timer
				TCNT1 = 0x01;
				break;
		// second click
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
		// disable external interrupt for B1
				GICR = 0 << 6;
				break;
		}
      inc++;
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
