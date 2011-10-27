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
#include "util.h"
#include "adc.h"


// define the contact sensors
#define CONTACT_LEFT    0b00000001  //PB0
#define CONTACT_RIGHT   0b00010000  //PB5
#define CONTACT_FRONT   0b00100000
#define CONTACT_BACK    0b00010000  //PB4

// define the line sensors
#define LINE_FRONT      0b00010000  //PD4
#define LINE_BACK_LEFT  0b00000100  //PD2
#define LINE_BACK_RIGHT 0b00000010  //PD4

#define IR_FRONT_LEFT   0b00100000  //PC5
#define IR_FRONT_RIGHT  0b00010000  //PC4
#define IR_BACK_LEFT    0b00001000  //PC3
#define IR_BACK_RIGHT   0b00000100  //PC2

#define IR_BLDG_FWD     0b00000010  //PC1
#define IR_BLDG_BACK    0b00000001  //PC0

#define SEP 10
#define DETECT_DISTANCE 30

/* */
char isContacted = 0;




// float get_speed() {
//
// 	//this is fail because you don't compensate for your own speed.
//
// 	dist_data[1] = dist_data[0];
// 	dist_data[0] = get_distance();
// 	return (dist_data[0]-dist_data[1] / SPEED_INTERVAL);
//
// 	//have to compensate for your own speed!!!
// }

float getDistanceFromSensor(int adc_channel) {
  if ((adc_channel < 0) || (adc_channel > 5)) { return 0; }
  adc_channel_select(adc_channel);
  return get_distance();
}

// left holds define for left sensor, likewise for right
int align(char left, char right, int leftVal, int rightVal){
  if (leftVal > rightVal) {
      while (abs(leftVal - rightVal) > SEP) {

	turnRight();
	_delay_ms(100);

	leftVal  = getDistanceFromSensor(left);
	rightVal = getDistanceFromSensor(right);
      }
      return 1;
    } else {
      while (abs(leftVal - rightVal) > SEP) {

	turnLeft();
	_delay_ms(100);

	leftVal  = getDistanceFromSensor(left);
	rightVal = getDistanceFromSensor(right);
      }

      return 1;

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

  if ((sensorVals[IR_FRONT_LEFT] > DETECT_DISTANCE) || (sensorVals[IR_FRONT_RIGHT] > DETECT_DISTANCE)) {
    if (abs(sensorVals[IR_FRONT_LEFT] - sensorVals[IR_FRONT_RIGHT]) < SEP) {

      moveForward();
      return 1;

    }

    return align(IR_FRONT_LEFT, IR_FRONT_RIGHT, sensorVals[IR_FRONT_LEFT], sensorVals[IR_FRONT_RIGHT]);

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

/********************************************************
 *
 *                   INTERRUPT VECTORS
 *			(bitches)
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
