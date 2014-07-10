 #define F_CPU 1000000UL
#define SPEED_INTERVAL 0.000225

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "motors.h"
#include "strategy.h"


// define the contact sensors
#define CONTACT_LEFT 0b00000001  //PB0
#define CONTACT_RIGHT 0b00010000 //PB5
#define CONTACT_FRONT 0b00100000
#define CONTACT_BACK 0b00010000  //PB4

// define the line sensors
#define LINE_FRONT 0b00010000 //PD4
//#define LINE_FRONT_RIGHT 0b00000100
#define LINE_BACK_LEFT 0b00000100 //PD2
#define LINE_BACK_RIGHT 0b00000010 //PD4

/* Sensor Data */

uint8_t adc_data[5];
static float dist_data[2];
const float voltages[] = {0.365, 0.552, 0.883, 1.276, 1.636, 2.303};
const int distances[]= {80, 50, 30, 20, 15, 10};

/* */
char isContacted = 0;

void blink();

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

/* initializes the ADC */
void adc_init(){
  DDRC = 0b00000000;
  DIDR0 = 0b00111111;

  ADCSRA = _BV(ADEN) | _BV(ADPS0); //enable adc
  //ADMUX |= _BV(REFS0) | _BV(ADLAR);
  ADMUX |= _BV(ADLAR);
}

/* clears the ADC interrupt flag */
void adc_clear_interrupt_flag(void)
{
  ADCSRA |= _BV(ADIF);
}

/* returns boolean value indicating if the ADC conversion is still in prgress */
int adc_conversion_in_progress(void){
  return !(ADCSRA & _BV(ADIF));
}

int get_adc(){
  uint8_t adc_value;        /* sensor ADC value */
  ADCSRA |= _BV(ADSC);      // start ADC conversion

  while(adc_conversion_in_progress());

  adc_value = ADCH;
  adc_clear_interrupt_flag();

  return(adc_value);
}

void adc_channel_select(int channel){
  if ((channel <= 5) && (channel >= 0)) {
    // set the channel in ADMUX register; left-align result

	//wait for conversion to complete.
	//while(adc_conversion_in_progress());
	//ADCSRA &= ~_BV(ADEN);
    //blink();
	//update register

	//_delay_ms(1);
	//channel = channel | _BV(REFS0) | _BV(ADLAR);
	channel = channel | _BV(ADLAR);
	ADMUX = channel;

	//re-enable adc
	//ADCSRA = _BV(ADEN) | _BV(ADPS0);

	// discard the first conversion
    //get_adc();
  }
	//_delay_ms(0);
  	/*
	switch (channel) {
	  case 0:
		ADMUX = 0b01100000;
		break;
	case 1:
		ADMUX = 0b01100001;
		break;
	}
	*/
}

/* get ADC Value */
int get_avg_adc(){

  int sum = 0;
  int avg;
  int i = 0;

  for(i=0; i < 10; i++){

    adc_data[i%5] = get_adc();
    sum += adc_data[i%5];
  }

  avg = sum/10;

  //_serial_write((unsigned char)(avg+0x30));

  return avg;
}



float get_distance(){

	//uint8_t distance;
	//uint8_t v1;
	//uint8_t d1;
	uint8_t adc_value = get_avg_adc();

	if (adc_value > voltages[0]) {
		int i =0;
		for (i=0; i < 5; i++) {

			if (adc_value < voltages[i]) {
				return (adc_value / voltages[i])*distances[i];
			}
		}
	}

	return 0;

}

float get_speed() {

	//this is fail because you don't compensate for your own speed.

	dist_data[1] = dist_data[0];
	dist_data[0] = get_distance();
	return (dist_data[0]-dist_data[1] / SPEED_INTERVAL);

	//have to compensate for your own speed!!!
}

float getDistanceFromSensor(int adc_channel) {
  if ((adc_channel < 0) || (adc_channel > 5)) { return 0; }
  adc_channel_select(adc_channel);
  return get_distance();
}

// float getDistanceFromSensor1(){
//
// 	adc_channel_select(0);
// 	return get_distance();
//
// }
//
// float getDistanceFromSensor2(){
//
// 	adc_channel_select(1);
// 	return get_distance();
//
// }
//
// float getDistanceFromSensor3(){
//
// 	adc_channel_select(2);
// 	return get_distance();
//
// }
//
// float getDistanceFromSensor4(){
//
// 	adc_channel_select(3);
// 	return get_distance();
//
// }

float building1(){

	adc_channel_select(4);
	return get_distance();

}

int building2(){

	adc_channel_select(5);
	return get_distance();

}

int alignFront(float left, float right){
  if (abs(left - right) < 2) {
    moveForward();
  }
  else {
    if (left > right) {
      while (abs(left - right) > 2) {
	turnRight();
      }
      return 1;
    } else {
      while (abs(right - left) > 2) {
	turnLeft();
      }
      return 1;
    }
  }
  return 0;
}

void align34(){


}

/* ********************
 * Checks analog sensors 1 by 1
 * ********************/

int poll_sensors(){
  // holds the sensors values
  float sensorVals[4] = {0,0,0,0};

  int i=0;
  for (i=0; i < 4; i++) {
    sensorVals[i] = getDistanceFromSensor(i);
  }

  if ((sensorVals[0] < 30) || (sensorVals[1] < 30)) {
    alignFront(sensorVals[0], sensorVals[1]);
  }
//   else if ((sensorVals[2] < 30) || (sensorVals[3] < 30)) {
//     align34(sensorVal[

//   if((getDistanceFromSensor1() < 30) || (getDistanceFromSensor2() < 30)){
//     align12();
//     if((getDistanceFromSensor1() < 30) && (getDistanceFromSensor2() < 30) && (building1() > 30))
//     {
//       return 1;
//     }
//   }
//   else if((getDistanceFromSensor4() < 30) || (getDistanceFromSensor3() < 30)){
//     align34();
//     if((getDistanceFromSensor3() < 30) && (getDistanceFromSensor4() < 30) && (building2() > 30))
//     {
//       return 2;
//     }
//   }
//   else{
//     return 0;
//   }
  return 0;
}



int rwContact(){

	if((PORTD&0b000000100) || (PORTD&0b000000001))
	{
		return 1;
	}
	else{

		return 0;
	}
}

int fwContact(){

	if((PORTD&0b100000000) || (PORTD&0b000010000)){


		return 1;
	}
	else{


		return 0;
	}
}

void evade() {

}

void attack() {

}

void setup_interrupts(){

	/*
		enable pin change interrupts on PORTB and PORTD
	*/

	//PCICR |= ~(_BV(PCIE1) && _BV(PCIE0) );
	//1<<2 ---> 00000010
	sei();
	PCICR |= 0b00000101;
	PORTD |= 0b00010110;
	/*
	//PCIFR ---> set when interrupt happens
	//PCIF0 --_> pin change on PORTB
	//PCIF1 ---> pin change on PORTC
	*/

		/*
		PIMSK bitches!!
		Lets us set which pins trigger interrupts.

		enable pin change interrupts on

		PB0 ---> PCINT0
		PB4 ---> PCINT4	        <---- Contact Sensors
		PB5 ---> PCINT5
		PB7 ---> PCINT6
		*/

	PCMSK0 |=0b10110001;

	/*
		PD1 ---> PCINT16
		PD2 ---> PCINT18        <---- Line Sensors
		PD4 ---> PCINT20
		PD7 ---> PCINT23
	*/

	PCMSK2 |=0b10010110;

}
void blink() {
  PORTD |= 0b00000001;
  _delay_ms(1000);
  PORTD &= 0b11111110;
  _delay_ms(1000);
}

/********************************************************
 *
 *                   INTERRUPT VECTORS
 * 						(bitches)
 *
 * *****************************************************/


// Contact switch is pressed or released
ISR(PCINT0_vect){
//   ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    _delay_ms(20);
//   }
  // left
  if(PINB & CONTACT_LEFT){
    turnRight();
    //isContacted = 1;
  }
  // right
  if(PINB & CONTACT_RIGHT){
   turnLeft();
   //isContacted = 1;
  }
  // back
  if(PINB & CONTACT_BACK){
    moveBackward();
    isContacted = 1;
    push(CONTACT_BACK);
  }
  // front
  if(PINB & CONTACT_FRONT){
    moveForward();
    isContacted = 1;
    push(CONTACT_FRONT);
  }
  PCIFR = 0b00000000;
}

ISR(PCINT2_vect){
  blink1();
  if (PIND & LINE_FRONT){
    //stop();
	//blink1();
	moveBackward();
    _delay_ms(3000);
    turnRight();
    _delay_ms(3000);
    moveForward();
  }
  //front right
  // if(~PORTD & LINE_FRONT_RIGHT){
    // moveBackward();
    // _delay_ms(1000);
    // turnLeft();
    // _delay_ms(2000);
    // moveForward();
  // }
  //back left
  else if(PIND & LINE_BACK_LEFT){
    stop();
	blink1();
	moveForward();
    _delay_ms(1000);
    turnRight();
    _delay_ms(2000);
    moveForward();
  }
  //back right
  else if(PIND & LINE_BACK_RIGHT){
    //stop();
	//blink1();
	moveForward();
    _delay_ms(1000);
    turnLeft();
    _delay_ms(2000);
    moveForward();
  }
  if (isContacted) {
    win();
  }
PCIFR = 0b00000000;
}
/*
void setupContactSensors(){

	DDRB &= 0b01001110;
	PORTB |= 0b10110001;

}
*/
