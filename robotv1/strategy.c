#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "strategy.h"
#include "sensors.h"

void push(char whichOne) {
  // keep doing whatever
  while (PINB & whichOne);
  // we lost them before an interrupt fired - poll sensors
  poll_sensors();
}

void win() {
}
