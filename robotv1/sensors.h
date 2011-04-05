#ifndef SENSORS_H
#define SENSORS_H

/* init ADC */
void adc_init();

/* get ADC Value */
int get_adc();

/* get distance corresponding to ADC value  */
float get_distance(int);

/*print sensor value*/
void distance_sensor_print(int,int);

/* get speed */
float get_speed();

/*get adc */
float get_avg_adc();

/*initialises ADC */
//void adc_init(void);

/* returns boolean value indicating if the ADC conversion is still in prgress */
//int adc_conversion_in_progress(void);

/* clears the ADC interrupt flag */
//void adc_clear_interrupt_flag(void);

float getDistanceFromSensor(int adc_channel);
float building_1();
float building_2();
int alignFront();
int fw_contact();
int rw_contact();
int poll_sensors();
void setup_interrupts();
void setupContactSensors();
void adc_channel_select(int);
void blink1();
void blink();

#endif
