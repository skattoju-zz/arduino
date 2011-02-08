char* itoa( uint8_t integer ) {

                /* convert ADC value to ascii for UART */
		char buffer[4]; 
                itoa (adc_value, buffer, 10);
		return (char*(buffer));

}
