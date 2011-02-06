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

int main() {

// setting up external interrupt to trigger at falling edge
      MCUCR |= 1<<1;
      MCUCR |= 1<<0;
	  //enable external interrupt
      GICR |= 0b1 << 6;

sei();
// setting up timer interrupt
          //TIMSK |= 1 << 0;
          TCCR1B = 0b010;

// setting up port C for LED output
      DDRC |= 0b11;
          PORTC |= 0b11;

// setting up port D for input
       DDRD = 0b0000;
// might be needed:
       //PORTD = 0b1100;

// initial blink
       turnOnBothLeds();
       _delay_ms(300);
       turnOffBothLeds();

// main loop that polls B2
     while(1){
		//poll = (PIND >> 3) & 0b1;
            if (((PIND & 0b00001000) == 0b00000000)) {
                
				_delay_ms(50);
				while (((PIND & 0b00001000) == 0b00000000)) {
					_delay_ms(1);
				}
                //toggle timer interrupts
                if(TIMSK & 0b00000001){
                    TIMSK = 0 << 0;
                } else {
					TIMSK = 1 << 2;
					TCNT0 = 0x01;
				}
				_delay_ms(100);
				
            }
       }


          return 0;
}

// interrupt service routine for B1 clicks
ISR(INT0_vect) {

	GICR = 0 << 6;
	   _delay_ms(200);
        GICR = 1 << 6;	

	      switch (inc) {
       // start alternation of LEDs
      case 0:
              turnOnLed2();
			  TIMSK = 1<<2;
			  TCNT1 = 0xA2C2;
              break;
       // turn LEDs on
      case 1:
	  		  TIMSK = 0;
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
       // GICR &= 0b00000000;
              break;
      }

	GICR &= 0b00111111;
	inc++;
	GICR = 1 << 6;
	GIFR = 1 << 6;
}

//timer interrupt service routine : makeas LEDs alternate every 5/11s
ISR(TIMER1_OVF_vect ) {

/*	
	if(PORTC & 0b11 == 0b11){

		PORTC &= ~0b01;
	}
*/
      PORTC = (~PORTC & 0b11) << 0;
	TCNT1 = 0xAF03; 
}

