#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "motors.h"
#include "sensors.h"
#include "serial.h"
#include <avr/interrupt.h>

#define MOTOR1A OCR0A //pin 12, PD6
#define MOTOR1B OCR0B //pin 11, PD5
#define MOTOR1E OCR2A //pin 17, PB3

#define MOTOR2A OCR1A //pin 15, PB1
#define MOTOR2B OCR1B //pin 16, PB2
#define MOTOR2E OCR2B //pin 5, PD3

#define MOTOR1A_PIN 6 //pin 12, PD6
#define MOTOR1B_PIN 5 //pin 11, PD5
#define MOTOR1E_PIN 3 //pin 17, PB3

#define MOTOR2A_PIN 1 //pin 15, PB1
#define MOTOR2B_PIN 2 //pin 16, PB2
#define MOTOR2E_PIN 3 //pin 5, PD3




int main(){

  DDRD  |= 0b00100001;
  PORTB |= 0b10110001;

  blink();

  //init adc
  adc_init();

  //enable motors
  //PORTB |= _BV(MOTOR1E_PIN);
  //PORTD |= _BV(MOTOR2E_PIN);

  sei();

  //setup_interrupts();

  //serial_init(9600);
  //blink();
  int val = 0;

  blink();

  while (1) {

	int i = 0;
	for(i=0; i <= 5 ; i++ ){

		adc_channel_select(i);
		val = get_adc();

		if(val > 50){

			PORTD &= 0b11111110;
		}
		else{

			PORTD |= 0b00000001;
		}
	//_delay_ms(1);
	}
	/*
	adc_channel_select(4);
	 val = get_adc();
	if(val > 100){
		blink1();
		_delay_ms(10);
		blink1();
	}
	*/

	/*
	adc_channel_select(3);
	 val = get_adc();
	if(val > 100){
		blink1();
		_delay_ms(10);
		blink1();
		_delay_ms(10);
		blink1();
	}
	*/

	/*
	adc_channel_select(2);
	 val = get_adc();
	if(val > 100){
		blink1();
		_delay_ms(10);
		blink1();
		_delay_ms(10);
		blink1();
		_delay_ms(10);
		blink1();
	}
		*/

		//while ((UCSR0A&(1<<UDRE0)) == 0);
		//UDR0 = 0x120;

	//PORTD &= 0b11111101;
    //if (UCSR0A & 0b01000000) { blink(); }
    // if (val > 0xA0) {
		// PORTD |= 0b00000001;
		// _delay_ms(3000);
		// PORTD &= 0b11111110;
		// _delay_ms(500);
	// }
//     moveForward();
//     _delay_ms(2000);
//     turnRight();
//     _delay_ms(2000);
//     turnLeft();
//     _delay_ms(2000);
  }
  return 0;
}
/*
ISR(USART_TX_vect) {
	blink();
}
*/