#define F_CPU 1000000UL  // 1 MHz
#define BAUD_RATE 4800   //serial comm baud rate
#define SPEED_TIMER 3

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include "led.h"
#include "serial.h"
#include "sensors.h"
#include "string.h"
#include "pwm.h"

#define F_CPU 1000000UL  // 1 MHz

int dist,speed;
uint8_t adc;

int main(){

	//sei();

//initializing a2d converter
	adc_init();

// initialize fast PWM on PB1 (OSCR1A)
	//setup_pwm();

// setting up serial communication 8E2 asynchronous mode.
// 4800 baud , 8 data bits , two stop bits, even parity.
	//serial_init(BAUD_RATE);
	serial_init(4800);

//initilize LEDs
	init_leds();
	TCCR0 = 0b101;
	TCNT0=0;
	TIMSK |= _BV(TOIE0);
	sei();
	TIMSK |= _BV(TOIE0);
// initial blink
startblink();
OCR1A = 0x0000;
// main loop that polls the distance sensor.
    while(1){
	  _serial_write('a');
	   while ((UCSRA & 0b01000000) == 0b00000000);
	   _delay_ms(150);
	}
	return 0;
}

ISR(TIMER0_OVF_vect){
	blink();
}
