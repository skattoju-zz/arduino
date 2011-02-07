#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h> 
#include <avr/interrupt.h>   
#include "serial.h"


/* initializes the ADC */
void adc_init(void)
{
        ADMUX = 0 | _BV(ADLAR); // channel 0, left-justified result
        ADCSRA = _BV(ADEN) | _BV(ADPS2);
}

/* starts an ADC conversion */
void adc_start_conversion(void)
{
    ADCSRA |= _BV(ADSC);
}

/* returns boolean value indicating if the ADC conversion is still in prgress */
int adc_conversion_in_progress(void)
{
    return !(ADCSRA & _BV(ADIF));
}

/* clears the ADC interrupt flag */
void adc_clear_interrupt_flag(void)
{
    ADCSRA |= _BV(ADIF);
}


/* get ADC Value */
void print_adc(){

		char buffer[4]; 
		uint8_t adc_value;        /* sensor ADC value */
		//uint8_t distance;       /* sensor distance in cm */

                adc_start_conversion();
                while(adc_conversion_in_progress());
                adc_value = ADCH;
                adc_clear_interrupt_flag();
                
                /* calculate the distance */
                //distance = gp2d120_adc8_to_cm(adc_value);
      
                /* convert ADC value to ascii for UART */
                itoa (adc_value, buffer, 10);
		serial_write('\t');
                serial_out((char*)buffer);
                                
                /* convert distance to ascii for UART
                itoa (distance, buffer, 10);
                uart_putc('\t');
                uart_puts(buffer);
                uart_puts_P("cm\n\r");
		*/
                
                /* long delay */
                //for (i=0; i<20; i++) { _delay_ms(250); }


}
