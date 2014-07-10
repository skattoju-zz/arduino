#include <avr/io.h>
#include "motors.h"

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

#define MOTOR_FULL 0xFF
#define MOTOR_STOP 0x00

void setupMotors(){

  // Set up DDR for pins used
  DDRD |= _BV(MOTOR1A_PIN) | _BV(MOTOR1B_PIN) | _BV(MOTOR2E_PIN);
  DDRB |= _BV(MOTOR2A_PIN) | _BV(MOTOR2B_PIN) | _BV(MOTOR1E_PIN);

  // All 3 timers are used
  // Timer 0: left motor A/B
  // Timer 1: right motor A/B
  // Timer 2: both motor enables

  // Register Description:
  /*** TCCR0A ***/
  // COM0A1   COM0A0   COM0B1   COM0B0   -   -   WGM01   WGM00
  //
  //
  // Compare output mode:
  // COM0A1   COMOA0   x=timer#,  y=A or B
  //   1        0      Clear OCOA on compare match
  //
  // waveform generator mode
  // WGM02  WGM01  WGM00    mode
  //   0      1      1     fast PWM
  //
  //
  /*** TCCR0B ***/
  // FOC0A   FOC0B   -   -   WGM02   CS02   CS01   CS00
  //
  //
  /*** TIMSK0 ***/
  //    -   -   -   -   -   OCIE0B   OCIE0A   TOIE0


  /* Fast PWM for timer 0, clear output compare on match */
  TCCR0A |= _BV(COM0A1) | _BV(COM0B1) | _BV(WGM00) | _BV(WGM01);
  TCCR0B |= _BV(CS00);
  TIMSK0 |= _BV(OCIE0A) | _BV(TOIE0);




  /*** TCCR1A ***/
  // COM1A1   COM1A0   COM1B1   COM1B0    -      -    WGM11   WGM10
  //
  /*** TCCR1B ***/
  // ICNC1    ICES1      -      WGM13   WGM12   CS12   CS11   CS10
  //
  // waveform generator mode
  // WGM13   WGM12   WGM11   WGM10        mode
  //   0       1       0       1    Fast PWM, 8 bit

  /* Fast PWM for timer 1, clear output compare on match */
  TCCR1A |= _BV(COM1A1) | _BV(COM1B1) | _BV(WGM10);
  TCCR1B |= _BV(WGM12) | _BV(CS10);
  //TIMSK1 |= _BV(OCIE1A) | _BV(TOIE1);

  // Register Description:
  /*** TCCR2A ***/
  // COM2A1   COM2A0   COM2B1   COM2B0   -   -   WGM21   WGM20
  //
  //
  // Compare output mode:
  // COM2A1   COM2A0   x=timer#,  y=A or B
  //   1        0      Clear OCOA on compare match
  //
  // waveform generator mode
  // WGM02  WGM01  WGM00    mode
  //   0      1      1     fast PWM
  //
  //
  /*** TCCR0B ***/
  // has clock select bits if we want them
  //
  //
  /*** TIMSK0 ***/
  //    -   -   -   -   -   OCIE0B   OCIE0A   TOIE0


  /* Fast PWM for timer 2, clear output compare on match */
  TCCR2A |= _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B |= _BV(CS20);
  //TIMSK2 |= _BV(TOIE2) | _BV(OCIE2A);
  /* why am I setting registers wierdly so that they take so much space ???? */

  /* reset all the timers and comparators */
//    OCR0A = 0xFF;
//    OCR0B = 0x00;
//   OCR1A = 0xFF;
//   OCR1B = 0xFF;
//   OCR2A = 0xFF;
//   OCR2B = 0xFF;

}

void motorLeft(int power, int direction) {
  if (direction == 0) { OCR0A = power; OCR0B = MOTOR_STOP; }
  if (direction == 1) { OCR0B = power; OCR0A = MOTOR_STOP; }
}

void motorRight(int power, int direction) {
  if (direction == 0) { OCR1A = power; OCR1B = MOTOR_STOP; }
  if (direction == 1) { OCR1B = power; OCR1A = MOTOR_STOP; }
}

void turnLeft() {
  PORTD |= _BV(MOTOR1A_PIN);
  PORTD &= ~_BV(MOTOR1B_PIN);
  PORTB &= ~(_BV(MOTOR2A_PIN) | _BV(MOTOR2B_PIN));
//   motorLeft(MOTOR_STOP, 1);
//   motorRight(MOTOR_FULL, 1);
}

void turnRight() {
  PORTB |= _BV(MOTOR2A_PIN);
  PORTB &= ~_BV(MOTOR2B_PIN);
  PORTD &= ~(_BV(MOTOR1A_PIN) | _BV(MOTOR1B_PIN));
//  motorLeft(MOTOR_FULL, 1);
//  motorRight(MOTOR_STOP, 1);
}

void moveForward() {
  PORTB |= _BV(MOTOR1E_PIN);
  PORTD |= _BV(MOTOR2E_PIN);

  PORTB &= ~(_BV(MOTOR2B_PIN));
  PORTB |= _BV(MOTOR2A_PIN);
  PORTD &= ~(_BV(MOTOR1B_PIN));
  PORTD |= _BV(MOTOR1A_PIN);

  //motorLeft(MOTOR_FULL, 1);
  //motorRight(MOTOR_FULL, 1);
}

void moveBackward() {
  PORTB |= _BV(MOTOR1E_PIN);
  PORTD |= _BV(MOTOR2E_PIN);

  PORTB &= ~(_BV(MOTOR2A_PIN));
  PORTB |= _BV(MOTOR2B_PIN);
  PORTD &= ~(_BV(MOTOR1A_PIN));
  PORTD |= _BV(MOTOR1B_PIN);

  //motorLeft(MOTOR_FULL, 1);
  //motorRight(MOTOR_FULL, 1);
}
void stop() {
  PORTB &= ~(_BV(MOTOR2A_PIN) | _BV(MOTOR2B_PIN));
  PORTD &= ~(_BV(MOTOR1A_PIN) | _BV(MOTOR1B_PIN));
}
