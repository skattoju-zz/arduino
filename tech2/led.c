#include <avr/io.h>

// LED enable disable
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

