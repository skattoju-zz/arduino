#ifndef SENSORS_H
#define SENSORS_H

/*initialises ADC */
void adc_init(void);

/* returns boolean value indicating if the ADC conversion is still in prgress */
int adc_conversion_in_progress(void);

/* clears the ADC interrupt flag */
void adc_clear_interrupt_flag(void);

/* get ADC Value */
uint8_t get_adc();

/* get distance corresponding to ADC value  */
float get_distance(int);

/*print sensor value*/
void distance_sensor_print(int,int);

float get_speed();

int get_avg_adc();

uint8_t calc_avg();

#endif
