#ifndef SENSORS_H
#define SENSORS_H

/*initialises ADC */
void adc_init(void);

/* starts ADC */
void adc_start_conversion(void);

/* returns boolean value indicating if the ADC conversion is still in prgress */
int adc_conversion_in_progress(void);

/* clears the ADC interrupt flag */
void adc_clear_interrupt_flag(void);

/* get ADC Value */
uint8_t get_adc();

#endif
