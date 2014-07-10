/*serial communications interface*/
#include <avr/io.h>
#include <string.h>
#include <stdlib.h>

#define F_CPU 1000000


void serial_init(unsigned int baud){
  DDRD |= 0b00000010;
  //  uint16_t i = (F_CPU/(32*baud))-1;
  //UBRRL = (unsigned char)(uint8_t)i;
  //UBRRH = (unsigned char)(uint8_t)(i >> 8);
  UBRR0L = 25;
  //UBRRH = 0 << URSEL;
  //UBRRH = 0;
  UCSR0A = 0 << 1;
  //UCSR0B |= (1 << TXEN0); // enabled
  UCSR0B = 0b11101000;

  //UCSR0C = 1 <<UMSELn1;
  UCSR0C = (1 << UMSEL01) | 0b10000110; // no parity, 8 data bits, 1 stop bits

}

void _serial_write(unsigned char c)
{


	//wait for UDR to be ready for data
	while ( !(UCSR0A & (1 << UDRE0)) ) {}

	//write data to UDR
	UDR0 = c;
}

void _serial_test(uint8_t data) {
	while ((UCSR0A&(1<<UDRE0)) == 0);
	UDR0 = data;
}

char serial_read(){

	// wait until data have been recieved and is ready to be read from UDR
	while ( !(UCSR0A & (1 << RXC0)) ){};

	// return the byte
	return UDR0;

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
