 #define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "motors.h"
#include "sensors.h"
#include "serial.h"
#include "util.h"
  
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




int main() {
  DDRD  |= 0b00110001;
  PORTB |= 0b10110001;
  blink();

  //init adc  
  adc_init();
  //enable motors
  PORTB |= _BV(MOTOR1E_PIN);
  //PORTD |= _BV(MOTOR2E_PIN);
  //PORTD &= 0b11111101;
  //disable interrupts
  //sei();
  //setup_interrupts();
  //serial_init(9600);
  

  
  //int val = 0;
  
  while (1) {
    
    blink();
    //poll_sensors();
    stop();
    moveForward();
    blink();
    _delay_ms(2000);
    turnRight();
    blink();
    _delay_ms(2000);
    turnLeft();
    blink();
    _delay_ms(2000);
  }
  return 0;
}

ISR(USART_TX_vect) {
	blink();
}
