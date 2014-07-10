/*serial communications interface*/
#include <avr/io.h>
#include <string.h>
#include <stdlib.h>
#include "string.h"

#define F_CPU 1000000UL


void serial_init(unsigned int baud){

	uint16_t i = (F_CPU/(8*baud))-1;

	// Load lower 8-bits of the baud rate value into the low byte of the UBRR register
  	UBRRL = i;
	// Load upper 8-bits of the baud rate value into the high byte of the UBRR register
  	UBRRH = i >> 8;
	//UBRRH &= 0b01111111;
	//UBRRH = 0x12;
	//UBRRH = BAUD_PRESCALE >> 8;
	//UBRRH |= 0b10000000;
	// Use 8-bit data bits and one stop bit and even parity.
  	//UCSRC = (1 << URSEL);   //select UCSRC
  	UCSRC = (1 << URSEL)   //select UCSRC
		|(1 << UCSZ1)
		|(1 << UCSZ0)  //8 data bits
		|(1 << UPM1);  //even parity
	// UCSRC = 0b10101110;

	// Enable TX and RX circuits
	UCSRB |= (1 << RXEN) | (1 << TXEN);

	return;
}

void serial_print(char* str[]) {
  blink();
  int i = 0;
  while (i < strlen(str)) {
    _serial_write((char)*str);
    i++;
  }
  _serial_write((char)"\n");
  _serial_write((char)"\r");


}


void _serial_write(unsigned char c)
{


	//wait for UDR to be ready for data
	while ( !(UCSRA & (1 << UDRE)) );

	//write data to UDR
	UDR = (uint8_t)c;
}

void _serial_write_number(uint8_t number)
{
  char* c;
	//wait for UDR to be ready for data
	while ( !(UCSRA & (1 << UDRE)) );
  itoa(number, c, 1);
	//write data to UDR
	serial_print(c);
}

void _serial_test(uint8_t data) {
	while ((UCSRA&(1<<UDRE)) == 0);
	UDR = data;
}

char serial_read(){

	// wait until data have been recieved and is ready to be read from UDR
	while ( !(UCSRA & (1 << RXC)) ){};

	// return the byte
	return UDR;

}

// void serial_print(const char* str){
//   _serial_write(*str);
// 	while(*str != '\0'){
//
// 		_serial_write(*str++);
// 	}
// 	_serial_write('\r');
// 	_serial_write('\n');
//
// }


void _serial_dbg(int val) {
//	char* ita;
	int i;
	unsigned char c;
	while (val > 0) {
		i = val % 10;
		if (i > 0) {
			c = (unsigned char)(i+48);
		} else {
			c = (unsigned char)48;
		}
		_serial_write(c);
		val = val/10;
	}
		// _serial_test('a');
	// serial_print((char*)ita);
}
