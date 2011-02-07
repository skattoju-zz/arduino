/*serial communications interface*/
#include <avr/io.h>

void serial_init(unsigned int BAUD_PRESCALE){

	// Load lower 8-bits of the baud rate value into the low byte of the UBRR register 
  	UBRRL = BAUD_PRESCALE; 
	// Load upper 8-bits of the baud rate value into the high byte of the UBRR register
  	UBRRH = (BAUD_PRESCALE >> 8);

	// Use 8-bit character sizes even parity  
  	UCSRC = (1<<URSEL)|(1<<USBS)|(1<<UCSZ1);

	// Enable TX and RX circuits
	UCSRB |= (1 << RXEN) | (1 << TXEN); 

	return;
}

void serial_write(unsigned char c)
{
	//wait for UDR to be ready for data
	while ( !(UCSRA & (1 << UDRE)) ){};

	//write data to UDR
	UDR = c;
}

char serial_read(){

	// wait until data have been recieved and is ready to be read from UDR
	while (!(UCSRA & (1 << RXC))) {}; 

	// return the byte
	return(UDR); 

}

void serial_out(char* str){

	int c = 0;
	while(str[c] != '\0'){
		serial_write(str[c]);
		c++;
	}

}
