char* itoa( int number ) {

                /* convert ADC value to ascii for UART */
		char buffer[4]; 
                itoa (number, buffer, 10);
		return (char*(buffer));

}
