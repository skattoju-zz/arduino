/*serial communications interface*/
#include <avr/io.h>
#include <string.h>
#include <stdlib.h>

#define F_CPU 1000000


void serial_init(unsigned int baud){

  //  uint16_t i = (F_CPU/(32*baud))-1;
  //UBRRL = (unsigned char)(uint8_t)i;
  //UBRRH = (unsigned char)(uint8_t)(i >> 8);
  UBRRL = 25;
  //UBRRH = 0 << URSEL;
  //UBRRH = 0;
  UCSRA = 0 << 1;
  UCSRB |= (1 << RXEN) | (1 << TXEN); // enabled
  UCSRB = 0b11111000;

  UCSRC = 1 << URSEL;
  UCSRC = (1 << URSEL) | 0b10000110; // no parity, 8 data bits, 1 stop bits 
  
}

void _serial_write(unsigned char c)
{

	
	//wait for UDR to be ready for data
	while ( !(UCSRA & (1 << UDRE)) );

	//write data to UDR
	UDR = (unsigned char)c;
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

void serial_print(char* str){

	while(*str != '\0'){

		//_serial_write(*str++);
	}
	//_serial_write('\r');
	//_serial_write('\n');

}

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
		//_serial_write((unsigned char)c);
		val = val/10;
	}
		_serial_test('a');
	// serial_print((char*)ita);
}
