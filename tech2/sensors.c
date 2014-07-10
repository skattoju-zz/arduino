#define F_CPU 1000000UL
#define SPEED_TIMER 3

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include "serial.h"
#include "led.h"
#include "inttypes.h"

/* Sensor Data */

uint8_t adc_data[5];
static float dist_data[2];
const float voltages[] = {0.365, 0.552, 0.883, 1.276, 1.636, 2.303};
const int distances[]= {80, 50, 30, 20, 15, 10};

static int dist1,dist2;

/* initializes the ADC */
void adc_init(void){
	DDRC = 0b0;
    //ADMUX = 0 | _BV(ADLAR); // channel 0, left-justified result
	ADMUX = 0b11100000;
    ADCSRA = _BV(ADEN) | _BV(ADPS0);
	ADCSRA |= _BV(ADSC);
	while (ADCSRA & (1 << ADSC));
	// _BV(bit) == (1 << bit)
}

// /* starts an ADC conversion */
// void adc_start_conversion(void){
	// ADCSRA |= _BV(ADSC);

// }

/* returns boolean value indicating if the ADC conversion is still in prgress */
int adc_conversion_in_progress(void){

    return !(ADCSRA & _BV(ADIF));
}

/* clears the ADC interrupt flag */
void adc_clear_interrupt_flag(void)
{
    ADCSRA |= _BV(ADIF);
}

uint8_t get_adc(){

		uint8_t adc_value;        /* sensor ADC value */

        ADCSRA |= _BV(ADSC); // start ADC conversion

		while(adc_conversion_in_progress());

		adc_value = ADCH;
		adc_clear_interrupt_flag();
		//adc_value = calc_avg(adc_value);

		return(adc_value);
}


uint8_t calc_avg(uint8_t adcval ) {
        uint8_t sum = 0;
		int i =0;
        for (i=0; i<4; i++) {
                adc_data[i+1] = adc_data[i];
        }

		adc_data[0] = adcval;
        for (i=0; i<5; i++) {
                sum += adc_data[i];
        }

        sum = sum/5;
        return sum;
}


/* get ADC Value */


int get_avg_adc(){


	int sum = 0;
	int avg;
	int i = 0;
	if (sum == 0) {
	while(i < 5){

		adc_data[i] = get_adc();
		sum += adc_data[i];
		i++;
	}
	}

	avg = sum/5;

	//_serial_write((unsigned char)(avg+0x30));

	return avg;
}

float get_voltage() {

}

float get_distance(){

	//uint8_t distance;
	//uint8_t v1;
	//uint8_t d1;
	uint8_t adc_value = calc_avg(get_adc());

	if (adc_value > voltages[0]) {
	int i =0;
	for (i=0; i < 5; i++) {
		if (adc_value < voltages[i]) {
			return (adc_value / voltages[i])*distances[i];
		}
	}
	}
	return 0;

}

float get_speed() {
	dist_data[1] = dist_data[0];
	dist_data[0] = get_distance();
	return (dist_data[0]-dist_data[1] / 0.000255);
}

void distance_sensor_print(int adc, int dist){

	char buffer[10];
	//serial_print((unsigned char*)"sensor value : \n\r\0");
	itoa(adc, buffer, 10);
	//serial_print((unsigned char*)buffer);
	//serial_print((unsigned char*)"distance : \n\r\0");
	itoa(dist, buffer, 10);
        //serial_print((unsigned char*)buffer);

}

void doIt() {
	uint8_t adcval = get_adc();
	uint8_t avg = calc_avg(adcval);
	uint8_t distance = 0;
}